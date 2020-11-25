
# Trader Bulk Sell Extension

Adds a new object "Bulk Sell Barrel" (Classname `Kat_Static_SellBarrel`)

This barrel has an action to "Sell all items" which will try to sell the contained items to any trader within 50m of the barrel.

Items will not be sold if...
- ruined
- remaining uses are lower than sold by the trader
- meat that has less than 50% remaining
- has no sell value

Attachments, ammunition in magazines and container contents are ignored. Empty all items before selling.

______

Requires Trader mod.

Inspired by Helkhiana's
TraderFixesAndFeatures and
MuchStuffPack .

Recommended to use TraderFixesAndFeatures with this mod.
