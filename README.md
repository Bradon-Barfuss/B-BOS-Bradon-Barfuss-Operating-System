# B-BOS
 B-BOS: Bradon Barfuss Operating System
 
 
Please visted this website for more information:
https://bradonbarfuss.wixsite.com/mywebsite

## Project B-BOS: My Operating System

### September 6, 2019

It all started back in September 2019 on our first Capstone Mentor Day. That day, I was thinking about what I could do for my capstone. I wanted to do something that I would be proud of and gain new skills and talents.

Turns out one simple thought started it all. About an hour before I met with my capstone teacher, I was trying to connect my wireless headphone to my computer. For some reason, Windows would not connect with them, and I remember thinking, "I wish I could make my own Operating system to not have these problems." And that started it all.

### Sources

#### September 9, 2019

- **Operating System Design and Implementation (3rd Edition)** by Andrew S. Tanenbaum
- For the first week, I was researching what an operating system is and resources I could learn from it. The first book I found that gave me the very fundmentals was the book "Operating Systems Design and Implementation (3rd Edition)" by Andrew S Tanenbaum.

#### September 16, 2019

- **os-Tutorial** by Carlos Fenollosa
- **Writing a Simple Operating System - from Scratch** by Nick Blundell
- After getting the concept on how an Operating System works and the fundmentals of it, I started the path of coding it. I first found a Git-hub page of a tutorial on how to make an Operating system by Carlos Fenollosa. He based his tutorial based on the document "Writing a Simple Operating System - from Scratch" by Nick Blundell

The Document "Writing a Simple Operating System - from Scratch" by Nick Blundell was what I mainly followed to get my first real step in the Operating System world. 
He covers:
- BIOS
- Boot sector
- The Magic Boot Number (0x7c00)
- 16-bit Real mode
- Assembly Code
- Reading from Disks
- Transiting into 32-bit protected mode
- The Global Descriptor Table (GDT)
- Makefiles
- Kernels

#### Transtion to Viktor Operating System
At this point, the tutorial ended and I had no idea of what to do. So after looking for more tutorials, I found the YouTube channel called "Write your own Operating System." This tutorial series by Viktor Engelmann got me to this point. I give a huge thanks to Viktor Engelmann for his amazing tutorial series and showed me the path to get to this point.

In his series he covers
- The boot order
- Grub
- Make files
- Assembly
- Link files
- Memory Segments
- The Global Descriptor Table (GDT)
- Interrupt Descriptor Table (IDT)
- Ports
- Programmable Interupt Controller (PIC)
- Interrupts
- The Keyboard
- The Mouse
- Abstract Hard Drivers
- Peripheral Component Interconnect (PCI)
- Setting Up Graphic Mode (VGA)
- Making a Graphic User Interface (GUI) framework
- Making a Desktop and separate Windows

#### December 4, 2019 ~ December 13, 2019

- **Write your own Operating System in 1 hour**

#### December 13, 2019 ~ December 19, 2019

- **Write your own Operating System 3: Memory Segments, Global Descriptor Table**

#### December 20, 2019 ~ December 26, 2019

- **Write your own Operating System 4: Hardware-Communication / Ports**

#### December 26, 2019 ~ January 7, 2020

- **Write your own Operating System 5: Interrupts**

#### January 7, 2020 ~ February 29, 2020

- **Write your own Operating System 12: Graphics Mode (VGA)**

### February 9, 2020 ~ April 28, 2020

This is the point where you can actually see progress in the Operating System. Yes, you could interact with it and display text beforehand. But you could not see any graphics. But I have finally activated the VGA mode and started making graphics!

In order to do this, I had to make a Graphic User Interface (GUI) to make the graphics.

That part was relatively easy, but setting up the VGA driver was very difficult because there is a lot of calculations you have to make to find what to initialize.

#### Features Implemented

- B-BOS runs in 32-bit protected mode.
- Has mouse and keyboard interface.
- Has its own GUI.

### Planned Additions

- Multitasking
- File system
- Networking

---

### All Sources

- [Operating System design and Implementation (3rd edition)](https://example.com)
- [os-Tutorial by Carlos Fenollosa](https://example.com)
- [Writing a Simple Operating System - from Scratch](https://example.com)
- [Oracle Assembly Values and Symbols Types](https://example.com)
- [From the bootloader to the kernel](https://example.com)
- [x86 Registers](https://example.com)
- [assembly x64 registers cheat sheet](https://example.com)
- [x86/win32 reverse engineering cheat-sheet](https://example.com)
- [Writing a bootloader part 1](https://example.com)
- [Write your own Operating System main Youtube Channel](https://example.com)
- [Write Your own Operating System main page](https://example.com)
- [Viktor Engelmann other website AlgorithMan.de](https://example.com)
- [Write your own Operating System in 1 hour](https://example.com)
- [Write your own Operating System 1-B: Addendum](https://example.com)
- [OSDev Boot Sequence](https://example.com)
- [OSDev Multiboot](https://example.com)
- [OSDev GRUB](https://example.com)
- [OSDev Bare Bones](https://example.com)
- [Write your own Operating System 3: Memory Segments, Global Descriptor Table](https://example.com)
- [OSDev Global Descriptor Table](https://example.com)
- [OSDev GDT Tutorial](https://example.com)
- [LowLevel Global Descriptor Table](https://example.com)
- [Brando Friesen The GDT](https://example.com)
- [Write your own Operating System 4: Hardware-Communication / Ports](https://example.com)
- [Write your own Operating System 5: Interrupts](https://example.com)
- [OSDev Interrupts](https://example.com)
- [OSDev Interrupts Tutorial](https://example.com)
- [OSDev Interrupt Descriptor Table](https://example.com)
- [Michal Lugvig Chapter 9 Exceptions and Interrupts](https://example.com)
- [Brando Friesen The IDT](https://example.com)
- [LowLevel Part 5 - Interrupts](https://example.com)
- [LowLevel Interrupt Descriptor Table](https://example.com)
- [Wikipedia Interrupt Descriptor Table](https://example.com)
- [Write your own Operating System 6: Keyboard](https://example.com)
- [Write your own Operating System 7: Mouse](https://example.com)
- [Write your own Operating System 8: Abstractions for Drivers](https://example.com)
- [Write your own Operating System 9: Tidying up](https://example.com)
- [Write your own Operating System 10: Peripheral Component Interconnect (PCI)](https://example.com)
- [Write your own Operating System 11: Base Address Registers](https://example.com)
- [OSDev PS/2 Keyboard](https://example.com)
- [OSDev PS/2 Mouse](https://example.com)
- [OSDev Mouse Input](https://example.com)
- [OSDev PCI](https://example.com)
- [LowLevel Keyboard Controller](https://example.com)
- [LowLevel PS 2 Mouse](https://example.com)
- [Sarathy Jayakumar System Architecture: 6 - PCI Basics and Bus Enumeration](https://example.com)
- [Yan Luo PCI bus](https://example.com)
- [Write your own Operating System 12: Graphics Mode (VGA)](https://example.com)
- [Write your own Operating System 13: GUI Framework Basics](https://example.com)
- [Write your own Operating System 14: Desktop and Windows](https://example.com)
- [OSDev VGA Hardware](https://example.com)
- [OSDev VGA fonts](https://example.com)
- [OSDev Scalable Screen Font](https://example.com)
- [OSDev Drawing In Protected Mode](https://example.com)
- [Hardware Level VGA and SVGA Video Programming](https://example.com)
- [256-Color VGA Programming in C](https://example.com)
- [Thor OS](https://example.com)
- [Chris Giese VGA-compatible video modes](https://example.com)
- [LowLevel Video Graphics Array](https://example.com)
- [Wikipedia Video Graphics Array](https://example.com)
- [Brandon Friesen: Bran's Kernel Development](https://example.com)
- Contact: [bradonbarfuss@gmail.com](mailto:bradonbarfuss@gmail.com)
