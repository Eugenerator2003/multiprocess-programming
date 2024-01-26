namespace dotnet
{
    internal class Program
    {
        static void Main(string[] args)
        {
            int threads = 4;
            var f = (double x) => Math.Exp(-x) * Math.Atan(x);
            var solver = new Solver(threads, f);
            double square = solver.Solve();
            Console.WriteLine($"Threads: {solver.ThreadCount}; Square: {square}; Time {solver.SolvingTime} milliseconds");
        }
    }
}