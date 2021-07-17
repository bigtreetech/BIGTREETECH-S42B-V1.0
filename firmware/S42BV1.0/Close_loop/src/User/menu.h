/*
 * A basic menu function
 * 
 * Contributors:
 * - Jan Swanepoel
 * 
 */
#include "stdio.h"
#include "stdbool.h"

#define MENU_MAX_ITEMS      12
#define MENU_SCREEN_ROWS    4

struct Menu;

enum MENU_ITEM_TYPE
{
    MENU_ITEM_TYPE_LABEL                = 0,
    MENU_ITEM_TYPE_VARIABLE_UINT8       = 1,
    MENU_ITEM_TYPE_VARIABLE_UINT16      = 2,
    MENU_ITEM_TYPE_ACTION               = 3,    // Calls action function
    MENU_ITEM_TYPE_ACTION_VAR_UINT8     = 4,    // Can edit variable but calls action when exiting edit mode  
};

// Function pointer type for drawing rows of text to the screen
typedef void(*drawFunc_t)(uint8_t x, uint8_t y, const char *text);

// Function pointer for clearing the screen
typedef void(*clearFunc_t)(void);

// Function pointer for action menu item types
typedef void(*actionFunc_t)(void);

// Function pointer for function that changes value 
typedef void(*valChangeFunc_t)(struct Menu*, int16_t);

// Function pointer for variable value converters
typedef uint16_t(*valConvertFunc_t)(uint16_t);


struct menuItemVariable
{
    void *value;        
    uint16_t maxValue;
    uint16_t minValue;
    // Function to modify value
    valChangeFunc_t change;
    // Used to convert value displayed
    valConvertFunc_t valueConverter;
};


struct menuItem
{
    // Title or lable that is displayed by this item
    const char* title;

    // Identify the type of menu item
    uint8_t type;

    // When true the variable can be edited
    bool inEditMode;
    
    // Holds the variable for variable type items
    struct menuItemVariable variable;

    // Points to the function to call for action menu items
    actionFunc_t action;
};


struct Menu
{
    // Array of menu items
    struct menuItem* items[MENU_MAX_ITEMS];

    // Number of menuItems in this menu
    uint8_t itemCount;                          

    // Current Y position of the cursor
    uint8_t cursorRow;

    // Currently active menu page
    uint8_t menuPage;

    // Index of the currently selected item in the item array
    uint8_t selectedItemIndex;

};


// Register the function which would be used to draw menu items
void Menu_Register_Draw(drawFunc_t drawFunc);

// Register the function which would be used to clear the screen
void Menu_Register_Clear(clearFunc_t clearFunc);

// Initialize the menu
void Menu_Init(struct Menu *menu);

// Initialize a menu item
void Menu_Item_Init(struct menuItem *item);

// Add menuItem to menu
void Menu_Add_Item(struct Menu *menu, struct menuItem *item);

// Draw the menu items
void Menu_Show(struct Menu *menu);

// Handle up button pressed
void Menu_Button_Down(struct Menu *menu);

// Handle down button pressed
void Menu_Button_Up(struct Menu *menu);

// Select the current menu item for editing
void Menu_Select_Edit(struct Menu *menu);