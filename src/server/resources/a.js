const files = [
    "SsdBuildingParameterTable",
"DefenseGameBaseDigging",
"DefenseGameQuest",
"SsdBaseDefenseSettings",
"SsdBuffParameter",
"SsdBuffParameter_debug",
"SsdCollectionItems",
"SsdCombatDeployParameterTable",
"SsdEquipParameters",
"SsdEquipParameters_debug",
"SsdEquipParameters_opt",
"SsdPlayerParameters",
"SsdReplayMissionInfo_opt",
"SsdWalkerGearParameters",
"SsdWeaponParameters",
"SsdWeaponParameters_debug",
"SsdWeaponParameters_opt",
"SsdCrewGeneratorTable",
"SsdNpcLevelParameters",
"SsdDamageParameter",
"SsdDamageParameter_debug",
"SsdDamageParameter_opt",
"DefenseGameEmbeddedMission",
"SsdSbmParameters",
"SsdSbmParameters_debug",
"SsdSbmParameters_opt",
"DefenseGameEventMission",
"SsdBuffParameter_opt",
"SsdDlcMissionSettings",
"SsdUiCoopMissionInfo",
]

const rename = (name) =>
{
    return name.replace(new RegExp(/([A-Z])/g), (x) => "_" + (x.toLowerCase())).slice(1)
}

for (const file of files)
{
    const name = rename(file)
    var hpp_file = `class ${name} : public parameter\r\n{\r\n\t${name}();\r\n};\r\n`
    console.log(`class ${} : public parameter\r\n{\r\n\t${rename(file)}();\r\n};\r\n`)
}