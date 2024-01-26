using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace multiprocess_lab4.Solving
{
    public class SingleThreadSolver : Solver
    {
        public override IEnumerable<LinearSystem> Solve(IEnumerable<LinearSystem> linearSystems)
        {
            stopwatch.Start();
            foreach (var system in linearSystems)
            {
                Solve(system);
            }
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
    }
}
