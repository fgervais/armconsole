# Introduction #
This game engine is designed to create side scroller games faster and easier. It will take care of the rendering process, the collision detection, the scrolling centered on the hero, enemy spawning etc. In the next section you'll find all the details that will hopefully put you in the game design process in no time.


# Details #

## Engine Modification ##

There is only one class that you need to modified in the engine directory and it's the Collider class. In it you should add a line for every sprite that can collide. That should look like this :

`virtual void collideWith(YourColliderClass*) {}`

This is the function that will be called when your sprite collide with some other sprite. In the case where you want to take actions like let say, decrease the energy level of your sprite, that's the function you will want to override. We'll look at that in detail the following sections.

## Creating your game ##

### Step 1 : Sprite subclass ###

The first thing you'll want to do in your game design process is to create a list of all the sprite you'll want in your game. Then create a class for every of these sprite in your game directory. These class should inherit from the Sprite class of the engine.

The implementation is up to you but these classes should normally contain the sound effects of your sprite in a static way so every instance of the same sprite share the same sound effects. Remember, you are memory and speed limited so you need to implement your objects in an optimized manner.

Do the same thing for the hero but as you'll want to control it, you should add some action functions to it like jump(), runRight(), runLeft() etc.

### Step 2 : States ###

### Step 3 : Game level creation ###

### Step 4 : Collision handling ###

## Engine Limitations ##

Here is a list of all the important thing you should know in order of the engine to work correctly.

### Background ###

Should be on screen high (272px) and 256px wide and repeat itself 3 times in the same image file. The final background image resolution should be 768x272. Don't worry this is the trickiest limitation. We do that because background scrolling is the hardest task so we want our DMA transfer to be as efficient as possible.

### Tiles ###

Should have a power of 2 size.

### Images ###

Should be in Bitmap format 24bpp.

### Sound ###

Should be a 16bit wave file 32kHz. This might expand in the future to include 8bit and other sample rate.