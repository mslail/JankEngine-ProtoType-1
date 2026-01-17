# JANK Engine - Windows API Window Creation

A minimal Windows application that creates and displays a window using the native Windows API.

## Overview

This code demonstrates the fundamental steps required to create a window in Windows: defining a window class, registering it with the system, creating the window, and running a message loop to handle user input and system events.

## Code Breakdown

### Header and Preprocessor Definitions

```cpp
#include <windows.h>
#define MAX_NAME_STRING 256
#define HInstance() GetModuleHandle(NULL)
```

**`#include <windows.h>`** includes the main Windows header file, which provides access to the entire Windows API including window creation functions, message handling, and GDI (Graphics Device Interface).

**`MAX_NAME_STRING`** defines a constant for the maximum length of string buffers used for the window class name and title.

**`HInstance()`** is a macro that retrieves the application's instance handle. `GetModuleHandle(NULL)` returns the handle of the current executable, which is the same value passed to `WinMain`. This macro allows access to the instance handle from anywhere in the code without passing it as a parameter.

### Global Variables

```cpp
WCHAR WindowClass[MAX_NAME_STRING];
WCHAR WindowTitle[MAX_NAME_STRING];
INT WindowWidth;
INT WindowHeight;
```

These global variables store the window configuration:

| Variable | Type | Purpose |
|----------|------|---------|
| `WindowClass` | `WCHAR[256]` | Unique identifier for the window class |
| `WindowTitle` | `WCHAR[256]` | Text displayed in the window's title bar |
| `WindowWidth` | `INT` | Window width in pixels |
| `WindowHeight` | `INT` | Window height in pixels |

`WCHAR` is a wide character type (16-bit Unicode) used for internationalization support.

### Entry Point

```cpp
int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
```

`WinMain` is the entry point for Windows GUI applications (equivalent to `main()` for console applications).

**Parameters (unnamed in this code):**
- `HINSTANCE hInstance` - Handle to the current instance of the application
- `HINSTANCE hPrevInstance` - Always NULL in modern Windows (legacy parameter)
- `LPSTR lpCmdLine` - Command line arguments as a string
- `INT nCmdShow` - How the window should be displayed (minimized, maximized, etc.)

**`CALLBACK`** specifies the calling convention (`__stdcall`), which defines how parameters are passed and the stack is cleaned up.

### Initializing Global Variables

```cpp
wcscpy(WindowClass, TEXT("JANK WindowClass"));
wcscpy(WindowTitle, TEXT("JANK WindowTitle"));
WindowWidth = 1366;
WindowHeight = 768;
```

**`wcscpy`** copies wide character strings. It's the Unicode version of `strcpy`.

**`TEXT()`** is a macro that makes string literals Unicode (`L"string"`) or ANSI (`"string"`) depending on project settings. Using `TEXT()` ensures compatibility with both configurations.

### Window Class Definition

```cpp
WNDCLASSEX wcex;
wcex.cbSize = sizeof(WNDCLASSEX);
```

`WNDCLASSEX` is an "extended" window class structure that defines the attributes of a window class. Windows uses this information when creating windows of this class.

**`cbSize`** must be set to the structure's size. Windows uses this for version compatibility, allowing the structure to be extended in future Windows versions without breaking existing applications.

### Window Class Fields

| Field | Value | Purpose |
|-------|-------|---------|
| `style` | `CS_HREDRAW \| CS_VREDRAW` | Redraws entire window if width or height changes |
| `cbClsExtra` | `0` | No extra bytes allocated for window class |
| `cbWndExtra` | `0` | No extra bytes allocated for window instance |
| `hCursor` | `LoadCursor(nullptr, IDC_ARROW)` | Standard arrow cursor |
| `hbrBackground` | `GetStockObject(NULL_BRUSH)` | No automatic background painting |
| `hIcon` | `LoadIcon(0, IDI_APPLICATION)` | Default application icon |
| `hIconSm` | `LoadIcon(0, IDI_APPLICATION)` | Small icon (taskbar, title bar) |
| `lpszClassName` | `WindowClass` | Name to identify this window class |
| `lpszMenuName` | `nullptr` | No menu attached to windows |
| `hInstance` | `HInstance()` | Application instance that owns this class |
| `lpfnWndProc` | `DefWindowProc` | Function that handles window messages |

### Key Concepts

**`CS_HREDRAW | CS_VREDRAW`** - These class styles use bitwise OR to combine flags. They tell Windows to redraw the entire client area whenever the window is resized horizontally or vertically.

**`NULL_BRUSH`** - A stock object that draws nothing. This is ideal for game engines or graphics applications where you handle all rendering yourself, preventing unnecessary background clearing.

**`DefWindowProc`** - The default window procedure provided by Windows. It handles standard window behavior (moving, resizing, closing, etc.). In a real application, you would create a custom `WndProc` function and call `DefWindowProc` for messages you don't handle.

### Registering the Window Class

```cpp
RegisterClassEx(&wcex);
```

This registers the window class with Windows. You must register a class before creating windows of that class. The `&` passes a pointer to the structure.

### Creating the Window

```cpp
HWND hWnd = CreateWindow(
    WindowClass,          // Registered class name
    WindowTitle,          // Window title bar text
    WS_OVERLAPPEDWINDOW,  // Window style
    CW_USEDEFAULT, 0,     // X, Y position
    WindowWidth,          // Width
    WindowHeight,         // Height
    nullptr,              // Parent window
    nullptr,              // Menu handle
    HInstance(),          // Application instance
    nullptr               // Additional data
);
```

**`HWND`** is a handle (pointer-like identifier) to the created window. Windows uses handles to track system resources.

**`WS_OVERLAPPEDWINDOW`** is a combination of styles that creates a typical application window with a title bar, system menu, minimize/maximize buttons, sizing border, and thick frame.

**`CW_USEDEFAULT`** tells Windows to choose a default position for the window.

### Error Handling

```cpp
if (!hWnd) {
    MessageBox(0, L"Failed to create a Window..", 0, 0);
    return 0;
}
```

If `CreateWindow` fails, it returns `NULL`. `MessageBox` displays an error dialog before exiting.

### Displaying the Window

```cpp
ShowWindow(hWnd, SW_SHOW);
```

A newly created window is hidden by default. `ShowWindow` makes it visible. `SW_SHOW` activates and displays the window in its current size and position.

### The Message Loop

```cpp
MSG msg = { 0 };
while (msg.message != WM_QUIT) {
    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
```

This is the heart of any Windows application. Windows communicates with your application through messages.

**`MSG`** is a structure that holds message information (message type, parameters, timestamp, etc.).

**`PeekMessage`** checks the message queue without blocking. Parameters:
- `&msg` - Pointer to receive the message
- `0` - Retrieve messages for any window owned by this thread
- `0, 0` - No message filtering (get all message types)
- `PM_REMOVE` - Remove the message from the queue after reading

**`TranslateMessage`** converts virtual-key messages (like `WM_KEYDOWN`) into character messages (`WM_CHAR`). Essential for keyboard input processing.

**`DispatchMessage`** sends the message to the appropriate window procedure (`DefWindowProc` in this case).

**Why `PeekMessage` instead of `GetMessage`?** `GetMessage` blocks until a message arrives, which is fine for typical applications. `PeekMessage` returns immediately even if no message is available, allowing the loop to continue. This is essential for games and real-time applications that need to update continuously, not just when user input occurs.

### Loop Termination

The loop exits when `msg.message` equals `WM_QUIT`, which is posted when the user closes the window. The application then returns 0 to indicate successful execution.

## Building the Project

Ensure your Visual Studio project is configured correctly:

1. **Subsystem**: Set to `Windows (/SUBSYSTEM:WINDOWS)` under Linker → System
2. **Character Set**: Set to `Unicode` under General → Character Set (for `WCHAR` and `TEXT()` to work correctly)

## Next Steps

To build a functional application, you would typically:

1. Create a custom window procedure to handle specific messages
2. Add rendering code (Direct3D, OpenGL, Vulkan) for graphics
3. Implement input handling for keyboard and mouse
4. Add a game loop or update logic inside the message loop's else branch