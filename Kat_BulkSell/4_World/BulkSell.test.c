
class BulkSellTest {

    
    // "unit test" -> these expect a default trader config
    static bool TestGetItemSellValue(PlayerBase player)
    {
        string itemName;
        int quantity;
        int expected;
        bool result = true;
        array<int> validTraders;

        // pretend all required traders are in range
        validTraders = { 0, 1, 2, 3, 4, 5, 6 };

        // "normal" item
        itemName = "TShirt_Black";
        quantity = 1;
        expected = 3;
        result = result && TestItem(player, itemName, quantity, expected, validTraders);

        // consumable full
        itemName = "BandageDressing";
        quantity = 4; // max is 4
        expected = 1;
        result = result && TestItem(player, itemName, quantity, expected, validTraders);

        // consumable too low
        itemName = "BandageDressing";
        quantity = 1; // max is 4
        expected = -1;
        result = result && TestItem(player, itemName, quantity, expected, validTraders);

        // ammo stack full
        itemName = "Ammo_9x39";
        quantity = -1; // max is 20
        expected = 20 * 2;
        result = result && TestItem(player, itemName, quantity, expected, validTraders);

        // ammo stack partial
        itemName = "Ammo_9x39";
        quantity = 2; // max is 20
        expected = 2 * 2;
        result = result && TestItem(player, itemName, quantity, expected, validTraders);

        // steak empty
        itemName = "GoatSteakMeat";
        quantity = 1; // max is 150
        expected = -1;
        result = result && TestItem(player, itemName, quantity, expected, validTraders);
        // steak full
        itemName = "GoatSteakMeat";
        quantity = -1; // = max
        expected = 1;
        result = result && TestItem(player, itemName, quantity, expected, validTraders);
        // steak above half
        itemName = "GoatSteakMeat";
        quantity = 80; // max is 150
        expected = 1;
        result = result && TestItem(player, itemName, quantity, expected, validTraders);

        // mag full
        itemName = "Mag_Glock_15Rnd";
        quantity = -1;
        expected = 20; // contained ammo is ignored
        result = result && TestItem(player, itemName, quantity, expected, validTraders);

        // mag partial
        itemName = "Mag_Glock_15Rnd";
        quantity = 5;
        expected = 20; // contained ammo is ignored
        result = result && TestItem(player, itemName, quantity, expected, validTraders);

        // mag empty
        itemName = "Mag_Glock_15Rnd";
        quantity = 0;
        expected = 20;
        result = result && TestItem(player, itemName, quantity, expected, validTraders);

        // TODO
        // weapon? with mag? attachments?
        // container non-empty?
        // maybe don't ignore ammo in non-empty mags?

        validTraders = { 0 }; // consume trader
        itemName = "Mag_Glock_15Rnd";
        quantity = 0;
        expected = -1; // trader 3 not in range
        result = result && TestItem(player, itemName, quantity, expected, validTraders);

        // second trader that is in range while the first trader for the item is NOT in range
        validTraders = { 1 }; // misc trader -> with added goat steak!
        itemName = "GoatSteakMeat";
        quantity = -1; // = max
        expected = 1;
        result = result && TestItem(player, itemName, quantity, expected, validTraders);

        if (result)
        {
            Kat_InfoPrint("All tests success");
        }
        return result;
    }

    static bool TestItem(PlayerBase player, string name, int quantity, int expected, array<int> validTraders)
    {
        // Arrange - Create a new item
        ItemBase item = ItemBase.Cast(GetGame().CreateObject(name, "0 0 0", false, false, false));
        if (quantity >= 0)
        {
            if (item.IsMagazine())
            {
                Magazine mag = Magazine.Cast(item);
                mag.ServerSetAmmoCount(quantity);
                mag.LocalSetAmmoCount(quantity);
            }
            else
            {
                item.SetQuantity(quantity);
            }
        }

        // Act
        int sell_value = BulkSell.GetItemSellValue(player, item, validTraders);

        // Assert expectations are met
        if (sell_value != expected)
        {
            Kat_ErrorPrint(string.Format(">>TEST FAILED>> %1, expected: %2, actual %3, quantitySet %4, quantityGet %5", name, expected, sell_value, quantity, QuantityConversions.GetItemQuantity(item)));
            return false;
        }
        return true;
    }
}
