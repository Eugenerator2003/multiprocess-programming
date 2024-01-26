using multiprocess_lab4.IO;
using System.IO.Pipes;

namespace Generator
{
    internal class GeneratorProgram
    {
        static void Main(string[] args)
        {
            IOUtil.CreateDirectory("f://laba6");
            Directory.SetCurrentDirectory("f://laba6");
            Console.WriteLine(Directory.GetCurrentDirectory());

            using (NamedPipeServerStream pipeServer =
            new NamedPipeServerStream("laba6", PipeDirection.Out))
            {
                Console.WriteLine("Named pipe server created");

                pipeServer.WaitForConnection();

                Console.WriteLine("Client connected.");
                try
                {
                    using (StreamWriter sw = new StreamWriter(pipeServer))
                    {
                        sw.AutoFlush = true;
                        int n = 10;
                        Generator.GenerateSystems(n);
                        sw.WriteLine($"Successfully generated {n}");
                    }
                }
                // Catch the IOException that is raised if the pipe is broken
                // or disconnected.
                catch (IOException e)
                {
                    Console.WriteLine("ERROR: {0}", e.Message);
                }
            }
        }
    }
}