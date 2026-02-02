# vLLM Ascend v0.11.0 正式发布

vLLM Ascend v0.11.0 版本现已正式发布！本次更新聚焦性能优化与稳定性提升，为昇腾用户带来更流畅的推理体验。

## ✨ 亮点更新

### 🚀 性能提升

**DeepSeek 3/3.1 系列性能显著提升**，满足高性能推理场景诉求，让大模型推理更加高效。

**采样操作性能优化**，提升整体推理效率，为用户带来更流畅的使用体验。

**Kimi-K2 模型性能优化**，改善用户体验。

### 🔥 功能恢复

**Eagle3 推测解码功能恢复支持**，为推理加速提供更多选择。

### 🐞 Bug 修复

本版本修复了多个关键问题：
- 修复 Qwen3-VL 精度问题，提升多模态推理准确性
- 修复 DeepSeek3.2-exp 量化bug
- 修复 Qwen3-VL-MOE 高并发场景稳定性问题
- 修复 Prefill-Decode 分离场景精度问题
- 修复 EPLB 相关bug，增强负载均衡稳定性
- 修复 openEuler Docker镜像版本兼容性问题

### 🔄 依赖更新

- torch-npu 升级至 2.7.1.post1
- CANN 升级至 8.3.rc2

### ⚠️ 废弃预告

以下功能将在 v0.12.0rc1 版本中移除，请提前做好迁移准备：
- LLMdatadist connector
- Torchair graph
- Ascend scheduler

## 📊 版本统计

- 本版本共计 **6 个 commits**
- 新增 **1 位**新开发者

## 🔗 相关链接

- GitHub 仓库：https://github.com/vllm-project/vllm-ascend
- Release 页面：https://github.com/vllm-project/vllm-ascend/releases/tag/v0.11.0

---

感谢所有贡献者的付出！欢迎大家试用新版本并提供反馈。

如有问题，欢迎在 GitHub 上提交 Issue 或参与讨论。
