# Net Host
This is literally just some testing I've done with hosting .NET Core in native applications.

Again, this is just me trying to invoke a managed method. This is a full-on mess, even the outputs aren't organized.

What this does, all in all, is it contains two projects - Native and TestModule:
- **Native** is a C++ project that will prompt the user to provide the path to the hostfxr.dll and the compiled TestModule's root.
- **TestModule** is a C# Code Library that only has one function, called Test, inside class Module, in namespace TestModule, that prints to stdout.

> [!IMPORTANT]
> **This is for educational purposes only.**
>
> The code is badly written and messy, very static and hard-coded and generally
> only shows how to approach accessing the .NET Core. I've only shared it as I believe it could potentially help someone
> in the future but it is not a good source. A great example of doing this properly would be Godot's implementation, and even
> that one has plenty that can be improved!

## Some Limitations
So, since this was really a concentrated test on just invoking a managed method from C++, there's plenty of things that
should probably be done and are not addressed at all in here (although they are surprisingly easy in theory):
- Automatically finding hostfxr.dll (and for the love of god - do not use `get_hostfxr_path`!!!)
- Calling functions with different parameters and return types
- Remove all hardcoding from there, possibly load data from the user or JSON or something else
- Allow loading DLLs without a runtimeconfig (this is actually really easy, just pretend the DLL is a console app, apparently!)
