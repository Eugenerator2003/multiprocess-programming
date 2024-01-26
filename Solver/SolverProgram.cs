using multiprocess_lab4;
using multiprocess_lab4.IO;
using multiprocess_lab4.Solving;
using System.IO.Pipes;
using System.Text.RegularExpressions;

namespace SolverProgram
{
    internal class SolverProgram
    {
        private static bool single = false;
        private static int threadCount = 8;
        private static Regex reg = new Regex(@"^Successfully generated \d+$");

        static void Main(string[] args)
        {
            IOUtil.CreateDirectory("f://laba6");
            Directory.SetCurrentDirectory("f://laba6");
            Console.WriteLine(Directory.GetCurrentDirectory());

            using (NamedPipeClientStream pipeClient = new NamedPipeClientStream(".", "laba6", PipeDirection.In))
            {
                pipeClient.Connect();

                Console.WriteLine("Connected to named pipe laba6");
                using (StreamReader sr = new StreamReader(pipeClient))
                {
                    string message;
                    while ((message = sr.ReadLine()) != null)
                    {
                        Console.WriteLine($"Message from generator process: {message}");
                        if (reg.IsMatch(message))
                        {
                            int n = int.Parse(message.Split(' ').Skip(2).First());
                            IEnumerable<LinearSystem> systems = IOUtil.ReadFromFiles(n);
                            Solver solver = null;
                            if (single)
                            {
                                solver = new SingleThreadSolver();
                            }
                            else
                            {
                                solver = new MultyThreadSolver(threadCount);
                            }
                            solver.Solve(systems);
                            if (single)
                            {
                                Console.WriteLine("Solved with single thread");
                            }
                            else
                            {
                                Console.WriteLine($"Solved with {threadCount} threads");
                            }
                            Console.WriteLine($"Time: {solver.Time} milliseconds");
                            IOUtil.CreateDirectory("results");
                            IOUtil.WriteArrays("results", systems.Select(s => s.X));
                        }
                    }
                }
            }
            Console.Write("Press Enter to continue...");
            Console.ReadLine();
        }
    }
}