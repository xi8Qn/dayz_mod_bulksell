
class ActionBulkSellForce : ActionBulkSell
{
    void ActionBulkSellForce()
    {
        super.init();
        m_force = true;
    }

    override string GetText()
    {
        return "Sell all items (even if no value)";
    }

};
