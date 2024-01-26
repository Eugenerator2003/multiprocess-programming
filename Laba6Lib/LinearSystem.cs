using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace multiprocess_lab4
{
    public class LinearSystem
    {
        public double[] Values { get; }
        public double[,] Matrix { get; }

        public double[] X { get; set; }

        public LinearSystem(double[] values, double[,] matrix)
        {
            Values = values;
            Matrix = matrix;
            X = new double[values.Length];
        }

        public LinearSystem(int members)
        {
            Values = new double[members];
            Matrix = new double[members, members];
            X = new double[members];
        }
    }
}
