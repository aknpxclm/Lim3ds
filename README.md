# Lim3ds
Recreation of Limbus Company for the 3DS console

- This project aims to recreate a basic version of limbus company containing the main gameplay from the orignal game.
- The main story found in Limbus Company will not be in the project for the forseeable future.
- I don't intend to replicate Limbus Company one-to-one, just the base game play and a small dialogue scene.
- This serves as my first full project as a beginner C programmer.

### Limbus Company
Limbus Company is a game developed by [Project Moon](https://limbuscompany.wiki.gg/wiki/Project_Moon) in 2023, which is a turn-based RPG.

## Source Files

The main source file is contained in the "source" folder, include files are in "libs/source".

"Tests" contains test files for code to be implemented in the main c, though this is rarely updated

P.S -
"gfx" currently contains images that are complied into a t3x spritesheet, noted by the .t3s file:
- Images must fit in a [1024x1024](https://github.com/devkitPro/tex3ds/issues/36) spritesheet when there are multiple images.

# Credits

Thanks to the Libctru team for creating and maintaining [Libctru](https://github.com/devkitPro/libctru) and [Devkitpro](https://devkitpro.org/) for maintaining the devkitarm compliers.

Now uses [cJSON](https://github.com/davegamble/cjson) as of 18/11/26 under the MIT licence.

## Notices

- All assets used in the application belong to [Project Moon](https://projectmoon.studio) and have been modified to be compatible with the 3DS' smaller resolution (and to save vram).
- I unfortunately won't be accepting pull requests or collaborations at the moment, but feel free to fork this instead.
