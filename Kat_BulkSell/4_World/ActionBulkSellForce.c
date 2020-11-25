class ActionBulkSellForceCB : ActionBulkSellCB
{
};

class ActionBulkSellForce : ActionBulkSell
{

    void ActionBulkSellForce()
    {
        m_CallbackClass = ActionBulkSellForceCB;
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
        return "Sell all items (even if no value)";
    }

    override typename GetInputType()
    {
        return ContinuousInteractActionInput;
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        Kat_DebugPrint("starting bulk sell - force");

        PlayerBase player = PlayerBase.Cast(action_data.m_Player);
        Kat_Static_SellBarrel container = Kat_Static_SellBarrel.Cast(action_data.m_Target.GetObject());

        BulkSell.SellAllItems(player, container, true);
    }
};
