using multiprocess_lab4.IO;
using multiprocess_lab4.Solving;

namespace multiprocess_lab4
{
    internal class Program
    {
        static void Main(string[] args)
        { 
            int n = 10;
            //IOUtil.GenerateSystems(n);

            IEnumerable<LinearSystem> singeLinearSystems = IOUtil.ReadFromFiles(n);
            IEnumerable<LinearSystem> multyLinearSystems = IOUtil.ReadFromFiles(n);

            var singeThreadSolver = new SingleThreadSolver();
            Console.WriteLine("Solving with single thread");
            singeThreadSolver.Solve(singeLinearSystems);
            Console.WriteLine(singeThreadSolver.Time + " milliseconds");

            var multyThreadSolver = new MultyThreadSolver(8);
            Console.WriteLine($"Solving with {multyThreadSolver.ThreadCount} thread");
            multyThreadSolver.Solve(multyLinearSystems);
            Console.WriteLine(multyThreadSolver.Time + " milliseconds");

            IOUtil.WriteArrays("single", singeLinearSystems.Select(s => s.X));
            IOUtil.WriteArrays("multy", multyLinearSystems.Select(s => s.X));
        }
    }
}