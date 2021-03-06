#include "interface_main.h"
#include "interface_building.h"

#include <utils_string.h>

void _pushBlock(block_t *block)
{ // Creates a new block and enters its context
	blockList_t *newBlockEntry = alloc(blockList_t);
	block_t *curBlock;

	curBlock = _curBlockStack->block;
	curBlock->children = (block_t**)Memory.reallocate(curBlock->children, sizeof(block_t*) * (curBlock->nbChildren+1));
	curBlock->children[curBlock->nbChildren++] = block;

	newBlockEntry->block = block;
	newBlockEntry->next = _curBlockStack;
	_curBlockStack = newBlockEntry;
}

void interface_pushBlock(placement_t placement)
{
	block_t *newBlock = alloc(block_t);
	Memory.set(newBlock, 0, sizeof(block_t));
	
	newBlock->position.placement = placement;

	_pushBlock(newBlock);
}

void interface_popBlock(void)
{ // Exits a block's context without destroying it
	blockList_t *curBlockEntry = _curBlockStack;

	_curBlockStack = _curBlockStack->next;

	dealloc(curBlockEntry);
}

int interface_newSideMenu(int x, int y, void (*onEnter)(uint), void (*onExit)(uint))
{ // Creates a menu at the given coordinates
	sideMenu_t *newMenu;

	if (_curBlockStack->next)
	{
		Console.print("Can't add a side menu while in a block!\n");
		return -1;
	}

	_interface.sidePanel.menus = (sideMenu_t*)Memory.reallocate(_interface.sidePanel.menus,sizeof(sideMenu_t)*(_interface.sidePanel.nbMenus+1));

	newMenu = &_interface.sidePanel.menus[_interface.sidePanel.nbMenus];
	Memory.set(newMenu, 0, sizeof(sideMenu_t));

	newMenu->content.position.screenPos.x = x;
	newMenu->content.position.screenPos.y = y;
	newMenu->content.position.screenPos.w = _interface.sidePanel.width;
	newMenu->content.position.screenPos.h = _interface.mainBlock.position.screenPos.h;

	newMenu->onEnter = onEnter;
	newMenu->onExit = onExit;

	_pushBlock(&newMenu->content);

	return _interface.sidePanel.nbMenus++;
}

void interface_closeSideMenu(void)
{ // :|
	interface_popBlock();
}

void _newRadioGroup(uint id)
{
	radioGroup_t *newGroup;

	_interface.radioGroups = (radioGroup_t*)Memory.reallocate(_interface.radioGroups, sizeof(radioGroup_t) * (_interface.nbRadioGroups+1));
	
	newGroup = &_interface.radioGroups[_interface.nbRadioGroups];
	newGroup->id = id;
}

radioGroup_t *_radioGroupWithID(uint id, bool createIfNotFound)
{
	uint i = 0;
	while (i < _interface.nbRadioGroups)
	{
		if (_interface.radioGroups[i].id == id)
		{
			return &_interface.radioGroups[i];
		}
	}

	if (createIfNotFound)
	{
		_newRadioGroup(id);
		return _radioGroupWithID(id, false);
	}

	return NULL;
}

void interface_setRadioGroupSelectionCallback(uint groupID, void (*onSelectionChanged)(uint index))
{
	radioGroup_t *group = _radioGroupWithID(groupID, true);
	group->onSelectionChanged = onSelectionChanged;
}

void _setObjectPlacement(uint objectID, placement_t placement)
{
	component_t *object = _interface.allComponents[objectID];
	object->position.placement = placement;
}

component_t *_addComponent(componentType_t type, placement_t placement)
{	
	component_t *newComp;

	_curBlockStack->block->components = (component_t**)Memory.reallocate(_curBlockStack->block->components, sizeof(component_t*)*(_curBlockStack->block->nbComponents+1));
	_interface.allComponents = (component_t**)Memory.reallocate(_interface.allComponents, sizeof(component_t*)*(_interface.nbComponents+1));

	newComp = newComponent(type, _interface.nbComponents);

	_curBlockStack->block->components[_curBlockStack->block->nbComponents++] = newComp;
	_interface.allComponents[_interface.nbComponents++] = newComp;

	newComp->position.placement = placement;
	return newComp;
}

int interface_staticLabel(char *text, placement_t placement, anchor_t anchor)
{
	component_t *component = _addComponent(COMP_LABEL, placement);
	label_t *label = component->gen_component.object.label;

	label->text = String.create(text);
	label->dynText = NULL;
	label->textAlignment = anchor;

	return component->id;
}

int interface_dynamicLabel(char **text, placement_t placement, anchor_t anchor)
{
	component_t *component = _addComponent(COMP_LABEL, placement);
	label_t *label = component->gen_component.object.label;

	label->text = NULL;
	label->dynText = text;
	label->textAlignment = anchor;

	return component->id;
}

int interface_staticButton(char *text, placement_t placement, void (*onClick)(void))
{
	component_t *component = _addComponent(COMP_BUTTON, placement);
	button_t *button = component->gen_component.object.button;
	
	button->text = String.create(text);
	button->dynText = NULL;
	button->onClick = onClick;

	return component->id;
}

int interface_textField(placement_t placement, fieldType_t type)
{
	component_t *component = _addComponent(COMP_TEXTFIELD, placement);
	textField_t *field = component->gen_component.object.textField;

	field->min = 0;
	field->max = 0;
	field->value = NULL;
	field->text = NULL;
	field->editable = 0;
	field->type = type;
	
	return component->id;
}

int interface_list(placement_t placement, anchor_t textAnchor, void (*onSelect)(uint))
{
	component_t *component = _addComponent(COMP_LIST, placement);
	list_t *list = component->gen_component.object.list;

	list->nbEntries = 0;
	list->entries = NULL;
	list->selectIndex = 0;
	list->startIndex = 0;
	list->textAlignment = textAnchor;
	list->onSelect = onSelect;

	// FIXME
	//list->maxDisplayedEntries = (int)(((float)placement-40.0)/15.0);
	list->maxDisplayedEntries = 500;

	return component->id;
}

int interface_slider(placement_t placement)
{
	component_t *component = _addComponent(COMP_SLIDER, placement);
	slider_t *slider = component->gen_component.object.slider;

	slider->percentage = 0.0f;
	slider->min = 0.0f;
	slider->max = 0.0f;
	slider->value = 0.0f;
	slider->dynValue = NULL;
	
	return component->id;
}

int interface_checkBox(placement_t placement)
{
	component_t *component = _addComponent(COMP_CHECKBOX, placement);
	checkBox_t *checkBox = component->gen_component.object.checkBox;

	checkBox->value = false;
	
	return component->id;
}

int interface_radioButton(placement_t placement, uint groupID)
{
	component_t *component = _addComponent(COMP_RADIOBUTTON, placement);
	radioButton_t *rButton = component->gen_component.object.radioButton;

	rButton->group = _radioGroupWithID(groupID, true);
	rButton->value = false;
	
	return component->id;
}

void interface_deleteObject(uint objectID)
{ // Deletes the selected object
	if (objectID >= _interface.nbComponents)
	{
		return;
	}

	destroyComponent(_interface.allComponents[objectID]);
}

void interface_deleteAllObjects(void)
{
	uint i;

	for (i = 0; i < _interface.nbComponents; i++)
	{
		if (_interface.allComponents[i]->gen_component.type != COMP_INACTIVE)
		{
			destroyComponent(_interface.allComponents[i]);
		}
	}
}

void interface_clearSideMenu(uint menuID)
{ // Removes all objects from the selected menu
	sideMenu_t *menu;

	if (menuID >= _interface.sidePanel.nbMenus)
	{
		return;
	}

	menu = &_interface.sidePanel.menus[menuID];
	
	if (menu->content.nbComponents > 0)
	{
		uint i;
		for (i = 0; i < menu->content.nbComponents; i--)
		{
			destroyComponent(menu->content.components[menu->content.nbComponents - i - 1]);
		}
	}
}

void interface_staticListEntry(uint listID, char *entry)
{
	list_t *list = _interface.allComponents[listID]->gen_component.object.list;
	
	list->entries = (listEntry_t*)Memory.reallocate(list->entries, sizeof(listEntry_t)*(list->nbEntries+1));

	list->entries[list->nbEntries].value = entry;
	list->entries[list->nbEntries].dynValue = NULL;
	
	list->nbEntries++;
	
	if (list->nbEntries == 1)
	{ // Got our first entry, select it
		list->startIndex = 0;
		listSelect(list, 0);
	}
}

void interface_clearList(uint listID)
{
	list_t *list = _interface.allComponents[listID]->gen_component.object.list;

	if (list->entries)
	{
		dealloc(list->entries);
		list->entries = NULL;
		list->nbEntries = 0;
	}
}

void interface_setSliderValue(uint sliderID, float min, float max, float *value)
{
	slider_t *slider = _interface.allComponents[sliderID]->gen_component.object.slider;
	slider->percentage = (*value-min)/(max-min);
	slider->dynValue = value;
	slider->min = min;
	slider->max = max;
}

void interface_setTextFieldValue(uint textFieldID, float min, float max, void **value, bool editable)
{
	textField_t *field = _interface.allComponents[textFieldID]->gen_component.object.textField;
	field->value = value;
	field->min = min;
	field->max = max;
	field->editable = editable;
	updateFieldText(field);
}

void interface_setCheckBoxValue(uint id, bool *value)
{
	checkBox_t *checkBox = _interface.allComponents[id]->gen_component.object.checkBox;
	checkBox->dynValue = value;
}

void interface_setRadioButtonValue(uint id, bool *value)
{
	radioButton_t *rButton = _interface.allComponents[id]->gen_component.object.radioButton;
	rButton->dynValue = value;
}

uint interface_getListSelectedIndex(uint id)
{
	return _interface.allComponents[id]->gen_component.object.list->selectIndex;
}

// Little hack to allow an entry to remain selected when rebuilding a list
uint _listID = 0;
void interface_saveListSelectedIndex(uint id)
{
	_listID = _interface.allComponents[id]->gen_component.object.list->selectIndex;
}

void interface_loadListSelectedIndex(uint id)
{
	listSelect(_interface.allComponents[id]->gen_component.object.list, _listID);
	_listID = 0;
}

void interface_updateLayout(void)
{
	updateBlockLayout(&_interface.mainBlock);
}

void initInterfaceFunctions(_interface_functions *UI)
{
	UI->checkBox = interface_checkBox;
	UI->clearList = interface_clearList;
	UI->clearSideMenu = interface_clearSideMenu;
	UI->closeSideMenu = interface_closeSideMenu;
	UI->deleteAllObjects = interface_deleteAllObjects;
	UI->deleteObject = interface_deleteObject;
	UI->dynamicLabel = interface_dynamicLabel;
	UI->getListSelectedIndex = interface_getListSelectedIndex;
	UI->list = interface_list;
	UI->loadListSelectedIndex = interface_loadListSelectedIndex;
	UI->newSideMenu = interface_newSideMenu;
	UI->popBlock = interface_popBlock;
	UI->pushBlock = interface_pushBlock;
	UI->radioButton = interface_radioButton;
	UI->saveListSelectedIndex = interface_saveListSelectedIndex;
	UI->setCheckBoxValue = interface_setCheckBoxValue;
	UI->setRadioButtonValue = interface_setRadioButtonValue;
	UI->setRadioGroupSelectionCallback = interface_setRadioGroupSelectionCallback;
	UI->setSliderValue = interface_setSliderValue;
	UI->setTextFieldValue = interface_setTextFieldValue;
	UI->slider = interface_slider;
	UI->staticButton = interface_staticButton;
	UI->staticLabel = interface_staticLabel;
	UI->staticListEntry = interface_staticListEntry;
	UI->textField = interface_textField;
	UI->updateLayout = interface_updateLayout;
}