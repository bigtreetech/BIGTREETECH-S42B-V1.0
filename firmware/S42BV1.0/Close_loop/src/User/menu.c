
#include "menu.h"


// Pointer to an external draw function
drawFunc_t draw;

// Pointer to an external clear function
clearFunc_t clear;

// Number conversion buffer
uint8_t numBuf[5];

// Default variable change function, adds the passed val to value
void Menu_Variable_Change(struct Menu *menu, int16_t val);

// Increase variable value
void Menu_Variable_Increase(struct Menu *menu);

// Decrease variable value
void Menu_Variable_Decrease(struct Menu *menu);

// Scroll to the next menu item
void Menu_Select_Next(struct Menu *menu);

// Scroll to the previous menu item
void Menu_Select_Previous(struct Menu *menu);

// Draw the menu item selection cursor
void Menu_Draw_Cursor(struct Menu *menu);


void UInt8ToStr(uint8_t val)
{
        numBuf[0] = '0';
        numBuf[1] = '0' + (val % 1000) / 100;
        numBuf[2] = '0' + (val % 100) / 10;
        numBuf[3] = '0' + (val % 10);
        numBuf[4] = 0;
}


void UInt16ToStr(uint16_t val)
{
    if (val < 9999)
    {
        numBuf[0] = '0' + val / 1000;
        numBuf[1] = '0' + (val % 1000) / 100;
        numBuf[2] = '0' + (val % 100) / 10;
        numBuf[3] = '0' + (val % 10);
        numBuf[4] = 0;
    }
    else
        numBuf[0] = 0;
}


void Menu_Draw_Cursor(struct Menu *menu)
{
    // Draw cursor
    if (menu->items[menu->selectedItemIndex]->inEditMode)
        draw(0, menu->cursorRow * 16, "*");
    else
        draw(0, menu->cursorRow * 16, ">");
}


void Menu_Register_Draw(drawFunc_t drawFunc)
{
    draw = drawFunc;
}


void Menu_Register_Clear(clearFunc_t clearFunc)
{
    clear = clearFunc;
}


void Menu_Init(struct Menu *menu)
{
    menu->itemCount = 0;
    menu->cursorRow = 0;
    menu->menuPage = 0;
    
    for (int i = 0; i < MENU_MAX_ITEMS; i++)
    {
        menu->items[i] = NULL;
    }  
}


void Menu_Item_Init(struct menuItem *item)
{
    item->variable.maxValue         = 9999;                     // Default maximum value
    item->variable.minValue         = 0;                        // Default minimum value
    item->variable.valueConverter   = NULL;                     
    item->variable.change           = &Menu_Variable_Change;    // Point to default change function
}


void Menu_Add_Item(struct Menu *menu, struct menuItem *item)
{
    if (menu->itemCount < MENU_MAX_ITEMS)
        menu->items[menu->itemCount++] = item;
}


void Menu_Show(struct Menu *menu)
{
    // Draw menu items
    for(int i = 0; i < MENU_SCREEN_ROWS; i++)
    {
        uint8_t itemIndex = i + (menu->menuPage * MENU_SCREEN_ROWS);

        if (itemIndex < menu->itemCount)
        {
            draw(12, (i * 16), menu->items[itemIndex]->title);
            
            if (menu->items[itemIndex]->type == MENU_ITEM_TYPE_VARIABLE_UINT16)
            {
                uint16_t val = *(uint16_t*)menu->items[itemIndex]->variable.value;
                
                // If variable has a value converter then use it
                if (menu->items[itemIndex]->variable.valueConverter != NULL)
                    val = menu->items[itemIndex]->variable.valueConverter(val);
                
                UInt16ToStr(val);
                draw(92, (i * 16), numBuf);
            }
            if ((menu->items[itemIndex]->type == MENU_ITEM_TYPE_VARIABLE_UINT8) ||
                (menu->items[itemIndex]->type == MENU_ITEM_TYPE_ACTION_VAR_UINT8))
            {
                uint16_t val = *(uint8_t*)menu->items[itemIndex]->variable.value;

                // If variable has a value converter then use it
                if (menu->items[itemIndex]->variable.valueConverter != NULL)
                    val = menu->items[itemIndex]->variable.valueConverter(val);
                
                UInt16ToStr(val);
                draw(92, (i * 16), numBuf);
            }
        }
    }

    Menu_Draw_Cursor(menu);
}


void Menu_Button_Up(struct Menu *menu)
{
    if (menu->items[menu->selectedItemIndex]->inEditMode)
        menu->items[menu->selectedItemIndex]->variable.change(menu, 1);
    else
        Menu_Select_Previous(menu);
}


void Menu_Button_Down(struct Menu *menu)
{
    if (menu->items[menu->selectedItemIndex]->inEditMode)
        menu->items[menu->selectedItemIndex]->variable.change(menu, -1);
    else
        Menu_Select_Next(menu);
}


void Menu_Variable_Change(struct Menu *menu, int16_t val)
{
    if ((menu->items[menu->selectedItemIndex]->type == MENU_ITEM_TYPE_VARIABLE_UINT8) ||
        (menu->items[menu->selectedItemIndex]->type == MENU_ITEM_TYPE_ACTION_VAR_UINT8))
    {
        // Check if increase would be above the maximum value
        if ((*(uint8_t*)menu->items[menu->selectedItemIndex]->variable.value + val) <= (menu->items[menu->selectedItemIndex]->variable.maxValue))
            if ((*(uint8_t*)menu->items[menu->selectedItemIndex]->variable.value + val) >= (menu->items[menu->selectedItemIndex]->variable.minValue))
                *(uint8_t*)menu->items[menu->selectedItemIndex]->variable.value += val;
    }
    if (menu->items[menu->selectedItemIndex]->type == MENU_ITEM_TYPE_VARIABLE_UINT16)
    {
        // Check if increase would be above the maximum value
        if ((*(uint16_t*)menu->items[menu->selectedItemIndex]->variable.value + val) <= (menu->items[menu->selectedItemIndex]->variable.maxValue))
            if ((*(uint16_t*)menu->items[menu->selectedItemIndex]->variable.value + val) >= (menu->items[menu->selectedItemIndex]->variable.minValue))
                *(uint16_t*)menu->items[menu->selectedItemIndex]->variable.value += val;
    }
}


void Menu_Select_Next(struct Menu *menu)
{
    if (menu->selectedItemIndex >= menu->itemCount - 1)
        return;

    // Clear current cursor
    draw(0, menu->cursorRow * 16, " ");

    menu->cursorRow++;

    if (menu->cursorRow >= MENU_SCREEN_ROWS)
    {
        if (menu->itemCount > ((menu->menuPage * MENU_SCREEN_ROWS) + menu->cursorRow))
        {
            // Go to next page
            menu->menuPage++;
            menu->cursorRow = 0;
            clear();
            Menu_Show(menu);
        }
        else
            menu->cursorRow = MENU_SCREEN_ROWS - 1;           
    }
    
    Menu_Draw_Cursor(menu);

    // Update selectedItemIndex
    menu->selectedItemIndex = (menu->menuPage * MENU_SCREEN_ROWS) + menu->cursorRow;
}


void Menu_Select_Previous(struct Menu *menu)
{
    // Clear current cursor
    draw(0, menu->cursorRow * 16, " ");

    menu->cursorRow--;

    if (menu->cursorRow > MENU_SCREEN_ROWS)
    {
        if (menu->menuPage > 0)
        {
            // Go to previous page
            menu->menuPage--;
            menu->cursorRow = MENU_SCREEN_ROWS - 1;
            clear();
            Menu_Show(menu);
        }
        else
            menu->cursorRow = 0;
    }

    Menu_Draw_Cursor(menu);
    
    // Update selectedItemIndex
    menu->selectedItemIndex = (menu->menuPage * MENU_SCREEN_ROWS) + menu->cursorRow;
}


void Menu_Select_Edit(struct Menu *menu)
{
    if (menu->items[menu->selectedItemIndex]->type == MENU_ITEM_TYPE_ACTION)
    {
        menu->items[menu->selectedItemIndex]->action();
        return;
    }

    if (menu->items[menu->selectedItemIndex]->inEditMode)
    {
        menu->items[menu->selectedItemIndex]->inEditMode = false;

        if (menu->items[menu->selectedItemIndex]->type == MENU_ITEM_TYPE_ACTION_VAR_UINT8)
            menu->items[menu->selectedItemIndex]->action();
    }
    else
        menu->items[menu->selectedItemIndex]->inEditMode = true;  

    Menu_Draw_Cursor(menu);   
}