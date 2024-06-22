"""
Simple 2d world where the player can interact with the items in the world.
"""

__author__ = "Sam Guarrera 45825594"
__date__ = "31/05/2019"
__version__ = "1.1.0"
__copyright__ = "The University of Queensland, 2019"

import tkinter as tk
import random
from collections import namedtuple

import pymunk

from block import Block, ResourceBlock, BREAK_TABLES, LeafBlock, TrickCandleFlameBlock
from grid import Stack, Grid, SelectableGrid, ItemGridView
from item import Item, SimpleItem, HandItem, BlockItem, MATERIAL_TOOL_TYPES, TOOL_DURABILITIES
from player import Player
from dropped_item import DroppedItem
from crafting import GridCrafter, CraftingWindow
from world import World
from core import positions_in_range
from game import GameView, WorldViewRouter
from mob import Bird
from tkinter import messagebox

BLOCK_SIZE = 2 ** 5
GRID_WIDTH = 2 ** 5
GRID_HEIGHT = 2 ** 4

# This global variable is for the values that certain items would mine. Upgraded axes will speed up the process.
BREAK_TABLES_2 = {
    "crafting_table": {
        "hand": (.75, True),
        "wood_axe": (.4, True),
        "stone_axe": (.2, True),
        "iron_axe": (.15, True),
        "diamond_axe": (.1, True),
        "golden_axe": (.1, True)
    }
}

# Task 3/Post-grad only:
# Class to hold game data that is passed to each thing's step function
# Normally, this class would be defined in a separate file
# so that type hinting could be used on PhysicalThing & its
# subclasses, but since it will likely need to be extended
# for these tasks, we have defined it here
GameData = namedtuple('GameData', ['world', 'player'])


def create_block(*block_id):
    """(Block) Creates a block (this function can be thought of as a block factory)

    Parameters:
        block_id (*tuple): N-length tuple to uniquely identify the block,
        often comprised of strings, but not necessarily (arguments are grouped
        into a single tuple)

    Examples:
        >>> create_block("leaf")
        LeafBlock()
        >>> create_block("stone")
        ResourceBlock('stone')
        >>> create_block("mayhem", 1)
        TrickCandleFlameBlock(1)
    """
    if len(block_id) == 1:
        block_id = block_id[0]
        if block_id == "leaf":
            return LeafBlock()
        # Task 2.3: Crafting table block created here
        elif block_id == 'crafting_table':
            return CraftingTableBlock(block_id, BREAK_TABLES_2[block_id])
        elif block_id in BREAK_TABLES:
            return ResourceBlock(block_id, BREAK_TABLES[block_id])

    elif block_id[0] == 'mayhem':
        return TrickCandleFlameBlock(block_id[1])



    raise KeyError(f"No block defined for {block_id}")


def create_item(*item_id):
    """(Item) Creates an item (this function can be thought of as a item factory)

    Parameters:
        item_id (*tuple): N-length tuple to uniquely identify the item,
        often comprised of strings, but not necessarily (arguments are grouped
        into a single tuple)

    Examples:
        >>> create_item("dirt")
        BlockItem('dirt')
        >>> create_item("hands")
        HandItem('hands')
        >>> create_item("pickaxe", "stone")  # *without* Task 2.1.2 implemented
        Traceback (most recent call last):
        ...
        NotImplementedError: "Tool creation is not yet handled"
        >>> create_item("pickaxe", "stone")  # *with* Task 2.1.2 implemented
        ToolItem('stone_pickaxe')
    """
    # Below is the code for tools that have two components (material and the type of weapon)
    if len(item_id) == 2:

        if item_id[0] in MATERIAL_TOOL_TYPES and item_id[1] in TOOL_DURABILITIES:
            return ToolItem(f'{item_id[1]}_{item_id[0]}', item_id[1], TOOL_DURABILITIES[item_id[1]])

    elif len(item_id) == 1:

        item_type = item_id[0]

        if item_type == "hands":
            return HandItem("hands")

        elif item_type == "dirt":
            return BlockItem(item_type)

        # Task 1.4 Basic Items: Create wood & stone here
        # ...
        elif item_type == "stone":
            return BlockItem(item_type)

        elif item_type == "wood":
            return BlockItem(item_type)

        # All the items that are created after 1.4 can be seen below
        elif item_type == "apple":
            return FoodItem(item_type, 2)

        elif item_type == "stick":
            return BlockItem(item_type)

        elif item_type == "knife":
            return ToolItem(item_type, "knife", 30)

        elif item_type == "bat":
            return ToolItem(item_type, "bat", 15)

        elif item_type == "spike_bat":
            return ToolItem(item_type, "spike_bat", 60)

        elif item_type == "crafting_table":
            return BlockItem(item_type)

        elif item_type == "fishing_rod":
            return ToolItem(item_type, "fishing_rod", 10)


    raise KeyError(f"No item defined for {item_id}")


# Task 1.3: Implement StatusView class here
# ...
class StatusView(tk.Frame):
    """The StatusView will create and update the "status bar" which will be visible under the display."""
    def __init__(self, master):
        super().__init__(master)

        # Import and pack the heart image to a label.
        self._heart_image = tk.PhotoImage(file='image_heart.png')
        self._heart_image_label = tk.Label(self, image=self._heart_image)
        self._heart_image_label.pack(side=tk.LEFT)

        # Create and pack the health next to the heart image.
        self._health = tk.Label(self, text="Health: 20.0")
        self._health.pack(side=tk.LEFT)

        # Import and pack the food image to a label.
        self._food_image = tk.PhotoImage(file='image_food.png')
        self._food_image_label = tk.Label(self, image=self._food_image)
        self._food_image_label.pack(side=tk.LEFT)

        # Create and pack the food next to the food image.
        self._food = tk.Label(self, text="Food: 20.0")
        self._food.pack(side=tk.LEFT)

    def rounding(self, number):
        """(float) The number that is entered will be multiplied by 2 and then rounded.  Then it will be divided by 2
        to return the rounded answer
        number (float): The number which is being rounded"""
        return float(round(number * 2) / 2)

    def set_health(self, health):
        """
        Updates the health bar by returning an updated version of it.
        Parameters:
            health(float): Current health of the player.
        """
        self._health.config(text=f'Health: {self.rounding(health)}')

    def set_food(self, food):
        """Updates the food bar by returning an updated version of it.
        Parameters:
            food(float): Current food of the player.
        """
        self._food.config(text=f'food: {float(self.rounding(food))}')


BLOCK_COLOURS = {
    'diamond': 'blue',
    'dirt': '#552015',
    'stone': 'grey',
    'wood': '#723f1c',
    'leaves': 'green',
    'crafting_table': 'pink',
    'furnace': 'black',
}

ITEM_COLOURS = {
    'diamond': 'blue',
    'dirt': '#552015',
    'stone': 'grey',
    'wood': '#723f1c',
    'apple': '#ff0000',
    'leaves': 'green',
    'crafting_table': 'pink',
    'furnace': 'black',
    'cooked_apple': 'red4'
}


def load_simple_world(world):
    """Loads blocks into a world

    Parameters:
        world (World): The game world to load with blocks
    """
    block_weights = [
        (100, 'dirt'),
        (30, 'stone'),
    ]

    cells = {}

    ground = []

    width, height = world.get_grid_size()

    for x in range(width):
        for y in range(height):
            if x < 22:
                if y <= 8:
                    continue
            else:
                if x + y < 30:
                    continue

            ground.append((x, y))

    weights, blocks = zip(*block_weights)
    kinds = random.choices(blocks, weights=weights, k=len(ground))

    for cell, block_id in zip(ground, kinds):
        cells[cell] = create_block(block_id)

    trunks = [(3, 8), (3, 7), (3, 6), (3, 5)]

    for trunk in trunks:
        cells[trunk] = create_block('wood')

    leaves = [(4, 3), (3, 3), (2, 3), (4, 2), (3, 2), (2, 2), (4, 4), (3, 4), (2, 4)]

    for leaf in leaves:
        cells[leaf] = create_block('leaf')

    for cell, block in cells.items():
        # cell -> box
        i, j = cell

        world.add_block_to_grid(block, i, j)

    world.add_block_to_grid(create_block("mayhem", 0), 14, 8)

    world.add_mob(Bird("friendly_bird", (12, 12)), 400, 100)


class Ninedraft:
    """High-level app class for Ninedraft, a 2d sandbox game"""

    def __init__(self, master):
        """Constructor

        Parameters:
            master (tk.Tk): tkinter root widget
        """

        self._master = master
        # Task 1.1 implemented here
        master.title("Ninedraft")
        self._world = World((GRID_WIDTH, GRID_HEIGHT), BLOCK_SIZE)

        load_simple_world(self._world)

        self._player = Player()
        self._world.add_player(self._player, 250, 150)

        self._world.add_collision_handler("player", "item", on_begin=self._handle_player_collide_item)

        self._hot_bar = SelectableGrid(rows=1, columns=10)
        self._hot_bar.select((0, 0))

        starting_hotbar = [
            Stack(create_item("dirt"), 20),
            Stack(create_item("apple"), 4)
        ]

        for i, item in enumerate(starting_hotbar):
            self._hot_bar[0, i] = item

        self._hands = create_item('hands')

        starting_inventory = [
            ((1, 5), Stack(Item('dirt'), 10)),
            ((0, 2), Stack(Item('wood'), 10)),
        ]
        self._inventory = Grid(rows=3, columns=10)
        for position, stack in starting_inventory:
            self._inventory[position] = stack

        self._crafting_window = None
        self._master.bind("e",
                          lambda e: self.run_effect(('crafting', 'basic')))

        self._view = GameView(master, self._world.get_pixel_size(), WorldViewRouter(BLOCK_COLOURS, ITEM_COLOURS))
        self._view.pack()

        # Task 1.2 Mouse Controls: Bind mouse events here
        # ...
        self._view.bind("<Motion>", self._mouse_move)
        self._master.bind("<Button-1>", self._left_click)
        self._master.bind("<Button-3>", self._right_click)

        # Task 1.3: Create instance of StatusView here
        # ...
        self._status_view = StatusView(master)
        self._status_view.pack(side=tk.TOP)

        self._hot_bar_view = ItemGridView(master, self._hot_bar.get_size())
        self._hot_bar_view.pack(side=tk.TOP, fill=tk.X)

        # Task 1.5 Keyboard Controls: Bind to space bar for jumping here
        # ...
        self._master.bind("<space>", lambda e: self._jump())

        self._master.bind("a", lambda e: self._move(-1, 0))
        self._master.bind("<Left>", lambda e: self._move(-1, 0))
        self._master.bind("d", lambda e: self._move(1, 0))
        self._master.bind("<Right>", lambda e: self._move(1, 0))
        self._master.bind("s", lambda e: self._move(0, 1))
        self._master.bind("<Down>", lambda e: self._move(0, 1))

        # Task 1.5 Keyboard Controls: Bind numbers to hotbar activation here
        # ...
        self._master.bind("1", lambda e: self._hot_bar.toggle_selection((0, 0)))
        self._master.bind("2", lambda e: self._hot_bar.toggle_selection((0, 1)))
        self._master.bind("3", lambda e: self._hot_bar.toggle_selection((0, 2)))
        self._master.bind("4", lambda e: self._hot_bar.toggle_selection((0, 3)))
        self._master.bind("5", lambda e: self._hot_bar.toggle_selection((0, 4)))
        self._master.bind("6", lambda e: self._hot_bar.toggle_selection((0, 5)))
        self._master.bind("7", lambda e: self._hot_bar.toggle_selection((0, 6)))
        self._master.bind("8", lambda e: self._hot_bar.toggle_selection((0, 7)))
        self._master.bind("9", lambda e: self._hot_bar.toggle_selection((0, 8)))
        self._master.bind("0", lambda e: self._hot_bar.toggle_selection((0, 9)))

        # Task 1.6 File Menu & Dialogs: Add file menu here
        # ...
        # The creation of the menu bar with the two commands being intitialised
        menubar = tk.Menu(master)
        master.config(menu=menubar)
        filemenu = tk.Menu(menubar)

        # The new game and exit need to be added to the menu.
        menubar.add_cascade(label="File", menu=filemenu)
        filemenu.add_command(label="New Game", command=self.new_game)
        filemenu.add_command(label="Exit", command=self.exit)

        # This will cause a pop up message if the window is closed down
        self._master.protocol("WM_DELETE_WINDOW", self.exit)

        self._target_in_range = False
        self._target_position = 0, 0

        self.redraw()

        self.step()

        # Task 2.2 Setting up the key for the crafting menu and variable for if crafting is open
        self._master.bind("e", lambda e: self._trigger_crafting("hands"))
        self._is_crafting_open = False

    # Task 1.6: Below are the functions which will be used for the command part of the menu
    def new_game(self):
        """The new_game function destroys the game window and creates a new one through main"""
        self._master.destroy()
        main()

    # The code #ed out below was another attempt at new_game that was attempting to reset it without closing the window
        # def new_game(self):
        # Re-load the world and add the player
        # load_simple_world(self._world)
        # self._world.add_player(self._player, 250, 150)

        # Refresh the hotbar
        # starting_hotbar = [
        #   Stack(create_item("dirt"), 20)
        # ]

        # for i, item in enumerate(starting_hotbar):
        #   self._hot_bar[0, i] = item

    def exit(self):
        """This function will exit out of the game.  It will give the user the message before exiting"""
        # The exit part was created using messagebox so that it would not need to be created the long way.
        exit_message = messagebox.askquestion(type=messagebox.YESNO,
                                              title="Closing Application",
                                              message="Are you sure you wish to exit Ninedraft?")
        if exit_message == messagebox.YES:
            self._master.destroy()

    def redraw(self):
        self._view.delete(tk.ALL)

        # physical things
        self._view.draw_physical(self._world.get_all_things())

        # target
        target_x, target_y = self._target_position
        target = self._world.get_block(target_x, target_y)
        cursor_position = self._world.grid_to_xy_centre(*self._world.xy_to_grid(target_x, target_y))

        # Task 1.2 Mouse Controls: Show/hide target here
        # ...
        if self._target_in_range:
            self._view.show_target(self._player.get_position(), self._target_position, cursor_position)
        else:
            self._view.hide_target()

        # Task 1.3 StatusView: Update StatusView values here
        # ...
        self._status_view.set_food(self._player.get_food())
        self._status_view.set_health(self._player.get_health())

        # hot bar
        self._hot_bar_view.render(self._hot_bar.items(), self._hot_bar.get_selected())

    def step(self):
        data = GameData(self._world, self._player)
        self._world.step(data)
        self.redraw()

        # Task 1.6 File Menu & Dialogs: Handle the player's death if necessary
        # ...
        if self._player.get_health() <= 0:
            death_message = messagebox.askquestion(type=messagebox.YESNO,
                                                   title="YOU DIED",
                                                   message="Do you wish to restart the application?")
            self._master.destroy()
            if death_message == messagebox.YES:
                main()

        self._master.after(15, self.step)

    def _move(self, dx, dy):
        self.check_target()
        velocity = self._player.get_velocity()
        self._player.set_velocity((velocity.x + dx * 80, velocity.y + dy * 80))

    def _jump(self):
        self.check_target()
        velocity = self._player.get_velocity()
        # Task 1.5: Update the player's velocity here
        # ...
        self._player.set_velocity((velocity.x / 1.5, velocity.y + 1 * -200))

    def mine_block(self, block, x, y):
        luck = random.random()

        active_item, effective_item = self.get_holding()

        was_item_suitable, was_attack_successful = block.mine(effective_item, active_item, luck)

        effective_item.attack(was_attack_successful)

        if block.is_mined():
            # Task 1.2 Mouse Controls: Reduce the player's food/health appropriately
            # ...
            if self._player.get_food() > 0:
                self._player.change_food(-1)
            elif self._player.get_health() > 0:
                self._player.change_health(-1)

            # Task 1.2 Mouse Controls: Remove the block from the world & get its drops
            # ...
            self._world.remove_block(block)
            drops = block.get_drops(luck, was_item_suitable)

            if not drops:
                return

            x0, y0 = block.get_position()

            for i, (drop_category, drop_types) in enumerate(drops):
                print(f'Dropped {drop_category}, {drop_types}')

                if drop_category == "item":
                    physical = DroppedItem(create_item(*drop_types))

                    # this is so bleh
                    x = x0 - BLOCK_SIZE // 2 + 5 + (i % 3) * 11 + random.randint(0, 2)
                    y = y0 - BLOCK_SIZE // 2 + 5 + ((i // 3) % 3) * 11 + random.randint(0, 2)

                    self._world.add_item(physical, x, y)
                elif drop_category == "block":
                    self._world.add_block(create_block(*drop_types), x, y)
                else:
                    raise KeyError(f"Unknown drop category {drop_category}")

    def get_holding(self):
        active_stack = self._hot_bar.get_selected_value()
        active_item = active_stack.get_item() if active_stack else self._hands

        effective_item = active_item if active_item.can_attack() else self._hands

        return active_item, effective_item

    def check_target(self):
        # select target block, if possible
        active_item, effective_item = self.get_holding()

        pixel_range = active_item.get_attack_range() * self._world.get_cell_expanse()

        self._target_in_range = positions_in_range(self._player.get_position(),
                                                   self._target_position,
                                                   pixel_range)

    def _mouse_move(self, event):
        self._target_position = event.x, event.y
        self.check_target()

    def _mouse_leave(self, event):
        self._target_in_range = False

    def _left_click(self, event):
        # Invariant: (event.x, event.y) == self._target_position
        #  => Due to mouse move setting target position to cursor
        x, y = self._target_position

        selected = self._hot_bar.get_selected()

        if not selected:
            return

        if self._target_in_range:
            block = self._world.get_block(x, y)
            if block:
                self.mine_block(block, x, y)

        selected = self._hot_bar.get_selected()

        # Will remove a item with 0 durability from the player's inventory
        selected_item = self._hot_bar[selected]
        if selected_item is not None and selected_item.get_item().get_durability() == 0:
            # remove item from hotbar
            self._hot_bar[selected] = None


    def _trigger_crafting(self, craft_type):
        # The first if statement checks if the window is already open.  If this is the case, it just closes the window
        if self._is_crafting_open:
            self._open_window.destroy()
            self._is_crafting_open = False

        # The below elif statement will open the 2 by 2 crafting menu.
        elif craft_type == "hands":
            self._is_crafting_open = True
            print(f"Crafting with {craft_type}")
            crafter = GridCrafter(CRAFTING_RECIPES_2x2, rows=2, columns=2)
            self._open_window = CraftingWindow(self._master, "Crafting", self._hot_bar, self._inventory, crafter)
            self._open_window.bind("e", lambda e: self._trigger_crafting("hands"))

        # The below code will open the 3 by 3 crafting menu
        else:
            self._is_crafting_open = True
            print(f"Crafting with {craft_type}")
            crafter = GridCrafter(CRAFTING_RECIPES_3x3, rows=3, columns=3)
            self._open_window = CraftingWindow(self._master, "Crafting", self._hot_bar, self._inventory, crafter)
            self._open_window.bind("e", lambda e: self._trigger_crafting("crafting_table"))

    def run_effect(self, effect):
        if len(effect) == 2:
            if effect[0] == "crafting":
                craft_type = effect[1]

                if craft_type == "basic":
                    print("Can't craft much on a 2x2 grid :/")

                elif craft_type == "crafting_table":
                    print("Let's get our kraft® on! King of the brands")

                self._trigger_crafting(craft_type)
                return
            elif effect[0] in ("food", "health"):
                # Changes need to be made here so that food will add to health.
                if self._player.get_max_health() > self._player.get_health():
                    x, strength = effect
                    stat = 'health'

                else:
                    stat, strength = effect
                print(f"Gaining {strength} {stat}!")
                getattr(self._player, f"change_{stat}")(strength)
            return

        raise KeyError(f"No effect defined for {effect}")

    def _right_click(self, event):
        print("Right click")

        x, y = self._target_position
        target = self._world.get_thing(x, y)

        if target:
            # use this thing
            print(f'using {target}')
            effect = target.use()
            print(f'used {target} and got {effect}')

            if effect:
                self.run_effect(effect)

        else:
            # place active item
            selected = self._hot_bar.get_selected()

            if not selected:
                return

            stack = self._hot_bar[selected]
            drops = stack.get_item().place()

            stack.subtract(1)
            if stack.get_quantity() == 0:
                # remove from hotbar
                self._hot_bar[selected] = None

            if not drops:
                return

            # handling multiple drops would be somewhat finicky, so prevent it
            if len(drops) > 1:
                raise NotImplementedError("Cannot handle dropping more than 1 thing")

            drop_category, drop_types = drops[0]

            x, y = event.x, event.y

            if drop_category == "block":
                existing_block = self._world.get_block(x, y)

                if not existing_block:
                    self._world.add_block(create_block(drop_types[0]), x, y)
                else:
                    raise NotImplementedError(
                        "Automatically placing a block nearby if the target cell is full is not yet implemented")

            elif drop_category == "effect":
                self.run_effect(drop_types)

            else:
                raise KeyError(f"Unknown drop category {drop_category}")

    def _activate_item(self, index):
        print(f"Activating {index}")

        self._hot_bar.toggle_selection((0, index))

    def _handle_player_collide_item(self, player: Player, dropped_item: DroppedItem, data,
                                    arbiter: pymunk.Arbiter):
        """Callback to handle collision between the player and a (dropped) item. If the player has sufficient space in
        their to pick up the item, the item will be removed from the game world.

        Parameters:
            player (Player): The player that was involved in the collision
            dropped_item (DroppedItem): The (dropped) item that the player collided with
            data (dict): data that was added with this collision handler (see data parameter in
                         World.add_collision_handler)
            arbiter (pymunk.Arbiter): Data about a collision
                                      (see http://www.pymunk.org/en/latest/pymunk.html#pymunk.Arbiter)
                                      NOTE: you probably won't need this
        Return:
             bool: False (always ignore this type of collision)
                   (more generally, collision callbacks return True iff the collision should be considered valid; i.e.
                   returning False makes the world ignore the collision)
        """

        item = dropped_item.get_item()

        if self._hot_bar.add_item(item):
            print(f"Added 1 {item!r} to the hotbar")
        elif self._inventory.add_item(item):
            print(f"Added 1 {item!r} to the inventory")
        else:
            print(f"Found 1 {item!r}, but both hotbar & inventory are full")
            return True

        self._world.remove_item(dropped_item)
        return False


# Task 2.1: FoodItem
class FoodItem(Item):
    """Deals with items which are food.  Food needs to be set so it cannot attack and if its eaten, it will give the
    player strength (health and food)"""

    def __init__(self, item_id: str, strength: float):
        super().__init__(item_id)
        self._strength = strength

    def get_strength(self):
        """(float) Returns a float which is the strength of the item """
        return self._strength

    def place(self):
        """(float) Returns an effect that represents an increase in the player's food/health"""
        return [('effect', ('food', self._strength))]

    def can_attack(self):
        """(bool) returns false as food cannot attack"""
        return False


# Task 2.1: ToolItem
class ToolItem(Item):
    """Deals with items which are tools.  Tools can attack and have a durability so this needs to be determined in this
    class."""

    def __init__(self, item_id: str, tool_type: str, durability: float):
        """"""
        super().__init__(item_id, max_stack=1)
        self._item_id = item_id
        self._tool_type = tool_type
        self._durability = durability
        self._max_durability = durability

    def get_type(self):
        """(str) returns the type of tool which is being used"""
        return self._tool_type

    def get_durability(self):
        """(float) returns the durability of the tool"""
        return self._durability

    def get_max_durability(self):
        """(float) returns the maximum durability of the tool which has been saved in the init"""
        return self._max_durability

    def can_attack(self):
        """(bool) returns true as a weapon can attack"""
        return True

    def attack(self, successful: bool):
        """If there is an attack, it should reduce the durability of the tool"""
        if not successful:
            self._durability -= 1


# Task 2.3: CraftingTableBlock
class CraftingTableBlock(ResourceBlock):
    """Class that deals with the crafting table block such as dropping it when mined and using it"""
    _id = 'crafting_table'

    _break_table = {
        "hand": (.35, False),
        "shears": (.4, True),
        "sword": (.2, False)
    }


    def get_drops(self, luck, correct_item_used):
        """Returns the drop of a crafting table if it is mined"""
        return [('item', (self._id,))]


    def use(self):
        """Returns the use of the crafting table"""
        return ("crafting", self._id)


# Task 2.2: Global variables for the crafting recipes.
# 2x2 Crafting Recipes
CRAFTING_RECIPES_2x2 = [
    (
        (
            (None, 'wood'),
            (None, 'wood')
        ),
        Stack(create_item('stick'), 4)
    )
    , (
        (
            (None, 'stick'),
            ('stick', None)
        ),
        Stack(create_item('knife'), 1)
    )
    , (
        (
            ('stone', None),
            ('stick', None)
        ),
        Stack(create_item('bat'), 1)
    )
    ,
    (
        (
            (None, None),
            ('knife', 'bat')
        ),
        Stack(create_item('spike_bat'), 1)
    )
,(
        (
            ('wood', 'wood'),
            ('wood', 'wood')
        ),
        Stack(create_item('crafting_table'), 1)
    )
]


# Task 2.3: Global variables for the crafting recipes.
# 3x3 Crafting Recipes
CRAFTING_RECIPES_3x3 = {
    (
        (
            (None, None, None),
            (None, 'wood', None),
            (None, 'wood', None)
        ),
        Stack(create_item('stick'), 16)
    ),
    (
        (
            ('wood', 'wood', 'wood'),
            (None, 'stick', None),
            (None, 'stick', None)
        ),
        Stack(create_item('pickaxe', 'wood'), 1)
    ),
    (
        (
            ('wood', 'wood', None),
            ('wood', 'stick', None),
            (None, 'stick', None)
        ),
        Stack(create_item('axe', 'wood'), 1)
    ),
    (
        (
            (None, 'wood', None),
            (None, 'stick', None),
            (None, 'stick', None)
        ),
        Stack(create_item('shovel', 'wood'), 1)
    ),
    (
        (
            (None, 'wood', None),
            (None, 'wood', None),
            (None, 'stick', None)
        ),
        Stack(create_item('sword', 'wood'), 1)
    ),
    (
        (
            (None, 'stick', None),
            (None, 'stick', None),
            (None, 'stick', None)
        ),
        Stack(create_item('fishing_rod'), 1)
    ),
    (
        (
            (None, 'stone', None),
            (None, 'stone', None),
            (None, 'stick', None)
        ),
        Stack(create_item('sword', 'stone'), 1)
    ),
    (
        (
            (None, 'stone', None),
            (None, 'stick', None),
            (None, 'stick', None)
        ),
        Stack(create_item('shovel', 'stone'), 1)
    )
}

# Task 1.1 App class: Add a main function to instantiate the GUI here
# ...
def main():
    """Launches the game."""
    root = tk.Tk()
    Ninedraft(root)
    root.mainloop()
    # root.protocol("WM_DELETE_WINDOW", Ninedraft.exit)


if __name__ == "__main__":
    main()
