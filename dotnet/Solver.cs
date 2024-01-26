using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dotnet
{
    internal class Solver
    {
        private struct ThreadParam
        {
            public double A, B;
        }

        public double A { get; set; }
        public double B { get; set; }
        public double Accuracy { get; set; }
        public Func<double, double> Function { get; set; }
        public int ThreadCount { get; set; }
        public long SolvingTime { get; set; }

        private readonly object _lock = new object();
        private int completed;
        private double square = 0;

        public Solver(int threadCount, Func<double, double> func) : this(threadCount, -1, 5, 0.00001, func)
        {
            
        }

        public Solver(int threadCount, double a, double b, double accuracy, Func<double, double> function)
        {
            A = a;
            B = b;
            Accuracy = accuracy;
            Function = function;
            ThreadCount = threadCount;
        }

        public double Solve()
        {
            square = 0;
            completed = 0;

            double step = (B - A) / ThreadCount;
            double x = A;

            Stopwatch stopwatch = new Stopwatch();
            stopwatch.Start();
            while(x < B)
            {
                Thread thread = new Thread(LeftRect);
                thread.Start(new ThreadParam { A = x, B = x + step });
                x += step;
            }
            while (completed < ThreadCount) ;
            stopwatch.Stop();
            SolvingTime = stopwatch.ElapsedMilliseconds;

            return square;
        }

        private void LeftRect(object data)
        {
            ThreadParam param = (ThreadParam)data;
            double prev, current;
            current = 0;
            int stepsCount = 2;
            do
            {
                prev = current;
                current = 0;

                double step = (param.B - param.A) / stepsCount;
                double x  = param.A;
                while(x <= param.B)
                {
                    current += Function(x);
                    x += step;
                }
                current *= step;

                stepsCount *= 2;
            }
            while (Math.Abs(current - prev) > Accuracy / 2 / ThreadCount);

            lock(_lock)
            {
                square += current;
                completed++;
            }
        }
    }
}
