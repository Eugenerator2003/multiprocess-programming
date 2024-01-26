using System;
using System.Drawing;
using System.IO.Pipes;
using System.Reflection.Emit;

namespace SmoothSolver
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Directory.SetCurrentDirectory(@"f:\laba5\Images");
            if (!Directory.Exists(@"..\Processed"))
            {
                Directory.CreateDirectory(@"..\Processed");
            }

            SmoothProcess smoothProcess = new SmoothProcess(6, 8);

            using (NamedPipeServerStream pipeServer = new NamedPipeServerStream("laba5", PipeDirection.Out))
            {
                Console.WriteLine("Named pipe server created");

                pipeServer.WaitForConnection();

                Console.WriteLine("Client connected.");
                try
                {
                    using (StreamWriter sw = new StreamWriter(pipeServer))
                    {
                        sw.AutoFlush = true;
                        foreach (var filename in Directory.GetFiles(Directory.GetCurrentDirectory()))
                        {
                            Bitmap bitmap = new Bitmap(filename);
                            Console.WriteLine($"Start smooth: {filename}");

                            smoothProcess.Process(bitmap);
                            Console.WriteLine($"Completed");

                            string name = filename.Split('\\').Last();
                            bitmap.Save(@"../Processed/" + name);

                            sw.WriteLine(name);
                            Console.WriteLine($"Sended to segmentator");
                        }
                        sw.WriteLine("ALL");
                    }
                }
                // Catch the IOException that is raised if the pipe is broken
                // or disconnected.
                catch (IOException e)
                {
                    Console.WriteLine("ERROR: {0}", e.Message);
                    Console.WriteLine("ERROR: {0}", e.StackTrace);
                }
            }
            Console.Write("Press Enter to continue...");
            Console.ReadLine();
        }
    }
}