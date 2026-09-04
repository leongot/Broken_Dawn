using UnrealBuildTool;

public class BrokenDawn : ModuleRules
{
    public BrokenDawn(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // Aggiunto "Niagara" tra le dipendenze pubbliche del modulo
        PublicDependencyModuleNames.AddRange(new string[] {
            "AIModule",
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "Niagara"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}