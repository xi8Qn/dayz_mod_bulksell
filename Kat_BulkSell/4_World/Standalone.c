
class Kat_Static_SellBarrel : Barrel_Green
{

    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionBulkSell);
    }

    override bool CanPutInCargo(EntityAI parent)
    {
        return false;
    }

    override bool CanPutIntoHands(EntityAI parent)
    {
        return false;
    }
};
