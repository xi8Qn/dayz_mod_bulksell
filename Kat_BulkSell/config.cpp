class CfgPatches
{
    class Kat_BulkSell
    {
        units[] =
            {
                "Kat_Static_SellBarrel"};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] =
            {"Trader"};
    };
};
class CfgMods
{
    class Kat_BulkSell
    {
        dir = "Kat_BulkSell";
        picture = "";
        action = "";
        hideName = 1;
        hidePicture = 1;
        name = "Kat_BulkSell";
        credits = "";
        author = "Katami";
        authorID = "0";
        version = "1.0";
        extra = 0;
        type = "mod";
        dependencies[] =
            {
                "Game",
                "World",
                "Mission"};
        class defs
        {
            class worldScriptModule
            {
                value = "";
                files[] =
                    {
                        "Kat_BulkSell/4_World"};
            };
            class missionScriptModule
            {
                value = "";
                files[] =
                    {
                        "Kat_BulkSell/5_Mission"};
            };
        };
    };
};
class CfgVehicles
{
    class Barrel_Green;
    class Kat_Static_SellBarrel : Barrel_Green
    {
        scope = 2;
        displayName = "Bulk Sell Barrel";
        descriptionShort = "A barrel to easily sell items. Cannot be picked up.";
        itemsCargoSize[] = {10, 50};
        itemSize[] = {5, 8};
        itemBehaviour = 2;
        // class AnimationSources
        // {
        //     class Doors1
        //     {
        //         source = "user";
        //         initPhase = 0;
        //         animPeriod = 1;
        //     };
        // };
    };
};
