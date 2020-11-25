
modded class MissionServer
{

    // also add required data to player object on server side
    void sendTraderDataToPlayer(PlayerBase player) {

        if(!GetGame().IsServer()){
            return;
        }

        super.sendTraderDataToPlayer(player);

        player.m_Trader_TraderIDs = new array<int>;
        player.m_Trader_ItemsTraderId = new array<int>;

        foreach (int traderId: m_Trader_TraderIDs )
        {
            player.m_Trader_TraderIDs.Insert(traderId);
        }
        
        foreach (int itemTraderId: m_Trader_ItemsTraderId )
        {
            player.m_Trader_ItemsTraderId.Insert(itemTraderId);
        }

    }

}
