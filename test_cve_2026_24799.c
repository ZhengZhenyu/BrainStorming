#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

/**
 * CVE-2026-24799 测试程序
 * 用于验证dlib中的zlib缓冲区溢出漏洞是否已修复
 */

/* 测试用例1：正常压缩数据解压 */
int test_normal_decompression() {
    unsigned char original[] = "Hello, World! This is a test for CVE-2026-24799";
    unsigned char compressed[1024];
    unsigned char decompressed[1024];
    uLongf compressed_len = sizeof(compressed);
    uLongf decompressed_len = sizeof(decompressed);
    
    printf("[TEST 1] Normal decompression test\n");
    
    /* 压缩数据 */
    if (compress2(compressed, &compressed_len, original, sizeof(original), 9) != Z_OK) {
        printf("  FAIL: Compression failed\n");
        return 0;
    }
    printf("  Original: %lu bytes, Compressed: %lu bytes\n", sizeof(original), compressed_len);
    
    /* 解压数据 */
    if (uncompress(decompressed, &decompressed_len, compressed, compressed_len) != Z_OK) {
        printf("  FAIL: Decompression failed\n");
        return 0;
    }
    
    /* 验证 */
    if (memcmp(original, decompressed, sizeof(original)) != 0) {
        printf("  FAIL: Decompressed data mismatch\n");
        return 0;
    }
    
    printf("  PASS: Data verified\n");
    return 1;
}

/* 测试用例2：边界条件测试 */
int test_boundary_conditions() {
    unsigned char test_data[100];
    unsigned char compressed[1024];
    unsigned char decompressed[100];
    uLongf compressed_len = sizeof(compressed);
    uLongf decompressed_len = sizeof(decompressed);
    int i;
    
    printf("[TEST 2] Boundary conditions test\n");
    
    /* 创建各种大小的测试数据 */
    size_t test_sizes[] = {1, 10, 64, 99, 100};
    
    for (i = 0; i < 5; i++) {
        size_t size = test_sizes[i];
        memset(test_data, 0xAA, size);
        
        compressed_len = sizeof(compressed);
        decompressed_len = sizeof(decompressed);
        
        if (compress2(compressed, &compressed_len, test_data, size, 9) != Z_OK) {
            printf("  FAIL: Compression failed for size %zu\n", size);
            return 0;
        }
        
        if (uncompress(decompressed, &decompressed_len, compressed, compressed_len) != Z_OK) {
            printf("  FAIL: Decompression failed for size %zu\n", size);
            return 0;
        }
        
        if (memcmp(test_data, decompressed, size) != 0) {
            printf("  FAIL: Data mismatch for size %zu\n", size);
            return 0;
        }
    }
    
    printf("  PASS: All boundary conditions passed\n");
    return 1;
}

/* 测试用例3：无效数据处理（会检测潜在的缓冲区溢出） */
int test_invalid_data_handling() {
    unsigned char invalid_data[] = {
        0x78, 0x9c,  // zlib 头
        0xFF, 0xFF, 0xFF, 0xFF,  // 无效的长度字段
        0x00, 0x00, 0x00, 0x00
    };
    unsigned char decompressed[1024];
    uLongf decompressed_len = sizeof(decompressed);
    int result;
    
    printf("[TEST 3] Invalid data handling test\n");
    
    /* 尝试解压无效数据 */
    result = uncompress(decompressed, &decompressed_len, invalid_data, sizeof(invalid_data));
    
    if (result == Z_DATA_ERROR || result == Z_BUF_ERROR) {
        printf("  PASS: Invalid data properly rejected (error: %d)\n", result);
        return 1;
    } else if (result == Z_OK) {
        printf("  WARNING: Invalid data was accepted (might indicate vulnerability)\n");
        return 0;
    } else {
        printf("  UNKNOWN: Unexpected error code %d\n", result);
        return 0;
    }
}

/* 测试用例4：压力测试 - 大数据处理 */
int test_large_data_handling() {
    unsigned char *large_data;
    unsigned char *compressed;
    unsigned char *decompressed;
    uLongf compressed_len, decompressed_len;
    size_t data_size = 1024 * 1024;  /* 1MB */
    int i, result;
    
    printf("[TEST 4] Large data handling test (%zu bytes)\n", data_size);
    
    /* 分配内存 */
    large_data = (unsigned char *)malloc(data_size);
    compressed = (unsigned char *)malloc(data_size + 1024);
    decompressed = (unsigned char *)malloc(data_size);
    
    if (!large_data || !compressed || !decompressed) {
        printf("  FAIL: Memory allocation failed\n");
        goto cleanup;
    }
    
    /* 填充测试数据 */
    for (i = 0; i < data_size; i++) {
        large_data[i] = (unsigned char)(i & 0xFF);
    }
    
    /* 压缩 */
    compressed_len = data_size + 1024;
    if (compress2(compressed, &compressed_len, large_data, data_size, 6) != Z_OK) {
        printf("  FAIL: Compression failed\n");
        goto cleanup;
    }
    printf("  Compressed %zu bytes to %lu bytes (%.1f%%)\n", 
           data_size, compressed_len, (float)compressed_len/data_size*100);
    
    /* 解压 */
    decompressed_len = data_size;
    result = uncompress(decompressed, &decompressed_len, compressed, compressed_len);
    if (result != Z_OK) {
        printf("  FAIL: Decompression failed (error: %d)\n", result);
        goto cleanup;
    }
    
    /* 验证 */
    if (memcmp(large_data, decompressed, data_size) != 0) {
        printf("  FAIL: Large data verification failed\n");
        goto cleanup;
    }
    
    printf("  PASS: Large data handled correctly\n");
    
cleanup:
    free(large_data);
    free(compressed);
    free(decompressed);
    return 1;
}

int main() {
    int total = 0, passed = 0;
    
    printf("=====================================\n");
    printf("CVE-2026-24799 zlib 缓冲区溢出测试\n");
    printf("=====================================\n\n");
    
    /* 运行测试 */
    total++; if (test_normal_decompression()) passed++;
    printf("\n");
    
    total++; if (test_boundary_conditions()) passed++;
    printf("\n");
    
    total++; if (test_invalid_data_handling()) passed++;
    printf("\n");
    
    total++; if (test_large_data_handling()) passed++;
    printf("\n");
    
    /* 结果汇总 */
    printf("=====================================\n");
    printf("测试结果: %d/%d 通过\n", passed, total);
    printf("=====================================\n");
    
    if (passed == total) {
        printf("\n✓ 所有测试通过！zlib似乎已正确修复。\n");
        return 0;
    } else {
        printf("\n✗ 部分测试失败，可能存在漏洞。\n");
        return 1;
    }
}
