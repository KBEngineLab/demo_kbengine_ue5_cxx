using System.IO;
using UnrealBuildTool;

public class LibhvModule : ModuleRules
{

    private void CopyDLLToBinariesDirectory(string Filepath, ReadOnlyTargetRules Target)
    {
        string BinariesPath = Path.Combine(ModuleDirectory,"../../../", "Binaries", Target.Platform.ToString());
        string FileName = Path.GetFileName(Filepath);

        if (!Directory.Exists(BinariesPath))
        {
            Directory.CreateDirectory(BinariesPath);
        }


        File.Copy(Filepath, Path.Combine(BinariesPath, FileName), true);

        RuntimeDependencies.Add(Path.Combine(BinariesPath,FileName));
    }

    public LibhvModule(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        Type = ModuleType.External;

        // 库目录
        PublicSystemLibraryPaths.Add(Path.Combine(ModuleDirectory, "Lib"));

        // 头文件
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "include"));

        // 添加依赖库
        PublicSystemLibraries.Add(Path.Combine(ModuleDirectory,"Lib","hv.lib"));

        // 动态库
        PublicDelayLoadDLLs.Add("hv.dll");

        CopyDLLToBinariesDirectory(Path.Combine(ModuleDirectory,"Lib","hv.dll"),Target);
        // RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "Lib","hv.dll"));
    }
}