class BulkSellItem {
    int m_index;
    int m_value;
}


modded class BulkSellMenu extends UIScriptedMenu
{
    // a menu that show all the items in the container and their calculated sell value or error information. + total sell value

    ButtonWidget m_BtnSell;
    // ButtonWidget m_BtnCancel;
    TextListboxWidget m_ItemsList;
    TextWidget m_AmountValue;

    override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets( "LayoutBulkSell/layouts/bulksell.layout" );

        m_BtnSell = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "sellButton" ) );
        // m_BtnCancel = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "cancelButton" ) );
        m_ListboxItems = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("itemsList") );
        m_AmountValue = TextWidget.Cast(layoutRoot.FindAnyWidget("text_saldoValue") );

        return layoutRoot;
    }

    override void OnShow()
    {
        PPEffects.SetBlurMenu(0.5);
        GetGame().GetInput().ChangeGameFocus(1);
        SetFocus( layoutRoot );
        GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
        GetGame().GetUIManager().ShowUICursor( true );
        // GetGame().GetMission().GetHud().Show( false );
    }

    override void OnHide()
    {
        PPEffects.SetBlurMenu(0);

        GetGame().GetInput().ResetGameFocus();

        if ( previewItem )
        {
            GetGame().ObjectDelete( previewItem );
        }
        GetGame().GetMission().PlayerControlEnable(false);
        GetGame().GetUIManager().ShowUICursor( false );
        // GetGame().GetMission().GetHud().Show( true );

        Close();
    }

    override bool OnClick( Widget w, int x, int y, int button )
    {
        PlayerBase m_Player = g_Game.GetPlayer();

        local int row_index = m_ListboxItems.GetSelectedRow();
        if(!m_FilteredListOfTraderItems.Get(row_index))
            return false;

        string itemType = m_FilteredListOfTraderItems.Get(row_index).ClassName;
        int itemQuantity = m_FilteredListOfTraderItems.Get(row_index).Quantity;

        if ( w == m_BtnSell )
        {
            // TODO sell all items...

            GetGame().GetUIManager().Back();
            return true;
        }

        // if ( w == m_BtnCancel )
        // {
        //     GetGame().GetUIManager().Back();

        //     return true;
        // }

        // if (w == m_XComboboxCategorys)
        // {
        //     if (updateListbox)
        //     {
        //         updateListbox = false;

        //         updateItemListboxContent();
        //         m_ListboxItems.SelectRow(0);

        //         updatePlayerCurrencyAmount();
        //         updateItemListboxColors();
        //     }
        // }

        return false;
    }

}
