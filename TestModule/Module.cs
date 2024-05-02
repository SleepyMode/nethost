
using System;

namespace TestModule
{
    public static class Module
    {
        public static int Test(IntPtr arg, int argLength)
        {
            Console.WriteLine("Hello, this is managed code! :D");
            return 0;
        }
    }
}
