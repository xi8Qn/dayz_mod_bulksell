
modded class PokeHolesBarrel extends RecipeBase
{
    override bool CanDo(ItemBase ingredients[], PlayerBase player)
    {
        // don't allow the sell barrel to be turned into a fire barrel :)
        return super.CanDo(ingredients, player) && !Kat_Static_SellBarrel.Cast(ingredients[0]);
    }
}
