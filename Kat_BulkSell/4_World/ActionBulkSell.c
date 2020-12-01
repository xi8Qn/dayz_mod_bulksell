class ActionBulkSellCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(1);
    }
};

class ActionBulkSell : ActionContinuousBase
{

    void ActionBulkSell()
    {
        Kat_InfoPrint("mod version 1.2.1"); // TODO move this somewhere else?
        m_CallbackClass = ActionBulkSellCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_CRAFTING;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINone;
        m_ConditionTarget = new CCTNone;
    }

    override string GetText()
    {
        return "Sell all items";
    }

    override typename GetInputType()
    {
        return ContinuousInteractActionInput;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        ItemBase container = ItemBase.Cast(target.GetObject());

        // Only show action if it's not locked or empty
        if (container && container.GetInventory().IsInventoryUnlocked() && container.GetNumberOfItems() > 0)
        {
            return true;
        }
        return false;
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        Kat_DebugPrint("starting bulk sell");

        PlayerBase player = PlayerBase.Cast(action_data.m_Player);
        ItemBase container = ItemBase.Cast(action_data.m_Target.GetObject());

        BulkSell.SellAllItems(player, container);
    }
};
