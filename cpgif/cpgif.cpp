#include <iostream>
#include <windows.h>
#include <vector>
#include <shlobj_core.h>
#include <locale>

int main(int argc, char** argv) {
    // 检查命令行参数是否包含GIF文件路径
    if (argc < 2) {
        return 1;
    }

    // 获取GIF文件路径
    const std::string gifFilePath = argv[1];

    // 创建文件列表
    std::vector<wchar_t> fileBuffer(gifFilePath.length() + 1);
    mbstowcs_s(nullptr, fileBuffer.data(), fileBuffer.size(), gifFilePath.c_str(), gifFilePath.length());

    // 创建一个HDROP结构，用于存储文件路径
    HGLOBAL hDrop = GlobalAlloc(GHND | GMEM_ZEROINIT, sizeof(DROPFILES) + (fileBuffer.size() + 1) * sizeof(wchar_t));
    DROPFILES* pDropFiles = (DROPFILES*)GlobalLock(hDrop);
    pDropFiles->pFiles = sizeof(DROPFILES);
    pDropFiles->fWide = TRUE;

    // 将文件路径复制到HDROP结构中
    memcpy((char*)pDropFiles + sizeof(DROPFILES), fileBuffer.data(), (fileBuffer.size() + 1) * sizeof(wchar_t));

    // 初始化剪贴板
    if (!OpenClipboard(NULL)) {
        GlobalFree(hDrop);
        return 1;
    }

    // 清空剪贴板内容
    EmptyClipboard();

    // 设置剪贴板数据为CF_HDROP格式
    SetClipboardData(CF_HDROP, hDrop);

    // 关闭剪贴板
    CloseClipboard();

    return 0;
}
