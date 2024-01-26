using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace multiprocess_lab4.Solving
{
    public abstract class Solver
    {
        protected Stopwatch stopwatch = new Stopwatch();

        // Время в миллисекундах
        public long Time { get; protected set; }

        public abstract IEnumerable<LinearSystem> Solve(IEnumerable<LinearSystem> linearSystems);
        public abstract LinearSystem Solve(LinearSystem linearSystem);
    }
}
