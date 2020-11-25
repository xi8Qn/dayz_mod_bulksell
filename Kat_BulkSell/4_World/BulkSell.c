

class BulkSell {

    static const int CONST_TRADER_SELL_DISTANCE = 50; // overwrite this to change the trader-barrel distance

    // magic error return values
    static const int ERROR_NULL = -1;
    static const int ERROR_TRADER_RANGE = -2;
    static const int ERROR_ITEM_NO_VALUE = -3;
    static const int ERROR_ITEM_ATTACHED = -4;
    static const int ERROR_ITEM_NOT_CONFIGURED = -5;
    static const int ERROR_ITEM_QUANTITY = -6;
    static const int ERROR_ITEM_RUINED = -7;

    static void SellAllItems(PlayerBase player, EntityAI container)
	{
        if (!player)
        {
            Kat_ErrorPrint("missing player");
            return;
        }

        if (player.m_Trader_RecievedAllData != true)
        {
            Kat_ErrorPrint("missing trader data!");
            return;
        }

        if (!container){
            Kat_ErrorPrint("invalid container");
            return;
        }

        CargoBase cargo = container.GetInventory().GetCargo();
        if (cargo == NULL){
            Kat_ErrorPrint("no target cargo");
            return;
        }

        array<int> validTraderIds = new array<int>;
        GetTradersInRange(player, validTraderIds);

        container.GetInventory().LockInventory(HIDE_INV_FROM_SCRIPT);

        int sold_items_count = 0;
        int total_sell_value = 0;
        int current_item = 0;
        int unsold_items_count = 0;
        while (true)
        {
            int item_count = cargo.GetItemCount();
            int remaining_items = item_count - current_item;
            Kat_DebugPrint(string.Format("remaining items: %1", remaining_items));
            if (remaining_items < 1)
            {
                break;
            }

            ItemBase item = ItemBase.Cast(cargo.GetItem(current_item));

            int itemPrice = SellItem(player, item, validTraderIds);

            if(itemPrice > 0)
            {
                total_sell_value += itemPrice;
                sold_items_count++;
            } 
            else
            {
                current_item++; // skip the item
                unsold_items_count++;
            }
        }

        container.GetInventory().UnlockInventory(HIDE_INV_FROM_SCRIPT);

        if (sold_items_count > 0)
        {
            // add money to player
            player.increasePlayerCurrency(total_sell_value);
            string message = string.Format("Sold %1 items for %2", sold_items_count, total_sell_value);

            Kat_DebugPrint(message);
            TraderMessage.PlayerWhite(message, player);
        }

        if(unsold_items_count > 0){
            string unsoldMessage = string.Format("%1 items could not be sold", unsold_items_count);
            Kat_DebugPrint(unsoldMessage);
            TraderMessage.PlayerRed(unsoldMessage, player);
        }

    }

    // Get all traders in range of the player
    static void GetTradersInRange(PlayerBase player, array<int> validTraderIds)
    {
        if (!player)
        {
            Kat_ErrorPrint("GetTradersInRange: No player");
            return;
        }
        if(!player.m_Trader_TraderIDs)
        {
            Kat_ErrorPrint("GetTradersInRange: missing m_Trader_TraderIDs");
            return;
        }        
        if(!player.m_Trader_TraderPositions)
        {
            Kat_ErrorPrint("GetTradersInRange: missing m_Trader_TraderPositions");
            return;
        }

        int traderIdCount = player.m_Trader_TraderIDs.Count();
        for (int i = 0; i < player.m_Trader_TraderPositions.Count(); i++)
        {
            vector pos = player.m_Trader_TraderPositions.Get(i);
            if (vector.Distance(player.GetPosition(), pos) <= CONST_TRADER_SELL_DISTANCE)
            {
                if(i >= traderIdCount){
                    Kat_ErrorPrint(string.Format("Missing traderId for traderPosition at index: %1. Skipping remaining traders.", i));
                    return;
                }
                int traderId = player.m_Trader_TraderIDs.Get(i);
                validTraderIds.Insert(traderId);
            }
        }
    }

    static int SellItem(PlayerBase player, ItemBase item, array<int> validTraderIds)
    {
        if (!item)
        {
            Kat_ErrorPrint("Invalid item reference");
            return ERROR_NULL; // try the next item
        }

        InventoryLocation myInvLoc = new InventoryLocation;
        if (!item.GetInventory().GetCurrentInventoryLocation(myInvLoc))
        {
            Kat_ErrorPrint("No item location");
            return ERROR_NULL; // try the next item
        }

        // find item in trader list
        int itemPrice = GetItemSellValue(player, item, validTraderIds);
        if (itemPrice <= 0)
        {
            Kat_DebugPrint("Can't sell item: no value");
            return itemPrice;
        }

        // delete the item
        Kat_DebugPrint(string.Format("deleting item %1", item));
        GetGame().ObjectDelete(myInvLoc.GetItem());

        return itemPrice;
    }

    static int GetItemSellValue(PlayerBase player, ItemBase item, array<int> validTraderIds)
    {
        if (!player)
        {
            Kat_ErrorPrint("GetItemSellValue: No player");
            return ERROR_NULL;
        }
        if(!player.m_Trader_ItemsClassnames)
        {
            Kat_ErrorPrint("GetItemSellValue: missing m_Trader_ItemsClassnames");
            return ERROR_NULL;
        }
        if(!player.m_Trader_ItemsTraderId)
        {
            Kat_ErrorPrint("GetItemSellValue: missing m_Trader_ItemsTraderId");
            return ERROR_NULL;
        }
        if(!player.m_Trader_ItemsSellValue)
        {
            Kat_ErrorPrint("GetItemSellValue: missing m_Trader_ItemsSellValue");
            return ERROR_NULL;
        }
        if(!player.m_Trader_ItemsQuantity)
        {
            Kat_ErrorPrint("GetItemSellValue: missing m_Trader_ItemsQuantity");
            return ERROR_NULL;
        }

        string itemName = item.GetType();
        Kat_DebugPrint(string.Format("item name: %1", itemName));

        int index = -1;
        int sell_price = -1;
        bool itemFound = false;
        
        foreach(int i, string name: player.m_Trader_ItemsClassnames) {
            if (itemName == name){
                itemFound = true; // item was found, but perhaps not in a nearby trader

                sell_price = player.m_Trader_ItemsSellValue.Get(i);
                if (!sell_price)
                {
                    Kat_DebugPrint("item has no sell price at this trader");
                    continue;
                }

                int itemTraderId = player.m_Trader_ItemsTraderId.Get(i);
                if (validTraderIds.Find(itemTraderId) < 0){
                    Kat_DebugPrint(string.Format("item found at trader %1, but too far -- ignoring", itemTraderId));
                    continue;
                }

                index = i;
                break; // done searching, can be sold at a nearby trader!
            }
        }
    
        // if the item was not found at all
        if (!itemFound)
        {
            Kat_ErrorPrint(string.Format("Item missing in trader data: %1", itemName));
            return ERROR_ITEM_NOT_CONFIGURED;
        }

        if (sell_price <= 0)
        {
            Kat_DebugPrint(string.Format("Item has no sell price: %1", itemName));
            return ERROR_ITEM_NO_VALUE;
        }

        if(index < 0){
            Kat_DebugPrint("item is not available at any nearby trader -- ignoring");
            return ERROR_TRADER_RANGE;
        }

        if (item.IsRuined())
        {
            Kat_DebugPrint("item is ruined, can't sell");
            return ERROR_ITEM_RUINED;
        }

        // TODO better check, and recursivly sell containers?
        if (player.isAttached(item))
        {
            Kat_DebugPrint("item is attached");
            return ERROR_ITEM_ATTACHED;
        }

        int trader_amount = player.m_Trader_ItemsQuantity.Get(index);
        int max_amount = player.GetItemMaxQuantity(itemName);

        float item_amount = GetQuantityModifier(item, trader_amount, max_amount);
        if (item_amount < 0)
        {
            Kat_DebugPrint(string.Format("Item amount too low: %1", item_amount));
            return ERROR_ITEM_QUANTITY;
        }

        int price_final = Math.Floor(sell_price * item_amount);
        Kat_DebugPrint(string.Format("priceSingle: %1, amount: %2, priceFinal: %3", sell_price, item_amount, price_final));

        return price_final;
    }

    static float GetQuantityModifier(ItemBase item, int trader_amount, int max_amount)
    {
        // TODO partially used items other than food? magazines?

        float item_amount = QuantityConversions.GetItemQuantity(item);
        int quantityType = QuantityConversions.HasItemQuantity(item); // QUANTITY_HIDDEN, QUANTITY_COUNT, QUANTITY_PROGRESS

        if (quantityType == QUANTITY_HIDDEN)
        {
            return 1;
        }

        bool isSteak = trader_amount == -5;
        if (isSteak)
        {
            if (item_amount < max_amount * 0.5)
            {
                Kat_DebugPrint("item is steak and too low quantity");
                return -1;
            }
            return 1;
        }

        bool isMagazine = trader_amount == -3;
        if (isMagazine)
        {
            item_amount = 1; // sell magazines at single price regardless of contained ammo ammount
        }

        if (quantityType == QUANTITY_PROGRESS)
        {
            // only sell progress items if they are above the trader amount
            if (item_amount > trader_amount)
            {
                return 1; // it's a full/unused item (or atleast above the value, the trader sells it)
            }

            Kat_DebugPrint("item is partially used or empty");
            return -1;
        }

        if (trader_amount > 1)
        {
            // if trader sells and buys multiple counts -> only return partial value
            return item_amount / trader_amount;
        }

        return item_amount;
    }
}
