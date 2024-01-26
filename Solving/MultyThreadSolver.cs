using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace multiprocess_lab4.Solving
{
    public class MultyThreadSolver : Solver
    {
        public int ThreadCount { get; }
        private Semaphore semaphore;
        private int counter;
        
        public MultyThreadSolver(int threadCount)
        {
            semaphore = new Semaphore(1, 1);
            ThreadCount = threadCount;
            ThreadPool.SetMaxThreads(ThreadCount, ThreadCount);
        }

        public override IEnumerable<LinearSystem> Solve(IEnumerable<LinearSystem> linearSystems)
        {
            int n = linearSystems.Count();
            stopwatch.Start();
            foreach(var system in linearSystems)
            {
                ThreadPool.QueueUserWorkItem(SolveOne, system);
            }
            while (counter < n) ;
            stopwatch.Stop();
            Time = stopwatch.ElapsedMilliseconds;
            return linearSystems;
        }

        public override LinearSystem Solve(LinearSystem linearSystem)
        {
            var values = linearSystem.Values.Clone() as double[];
            var ld = linearSystem.Matrix.Clone() as double[,];
            LdlMethod.Decomposition(ld);
            linearSystem.X = LdlMethod.Solve(ld, values);
            return linearSystem;
        }

        private void SolveOne(object stateInfo)
        {
            var linearSystem = stateInfo as LinearSystem;
            var values = linearSystem.Values.Clone() as double[];
            var ld = linearSystem.Matrix.Clone() as double[,];
            LdlMethod.Decomposition(ld);
            linearSystem.X = LdlMethod.Solve(ld, values);

            semaphore.WaitOne();
            counter += 1;
            semaphore.Release();
        }
    }
}
