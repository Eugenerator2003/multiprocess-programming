using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace multiprocess_lab4.Solving
{
    public class LdlMethod
    {
        public static void Decomposition(double[,] LD)
        {
            int n = LD.GetLength(0);
            for (int j = 0; j < n; j++)
            {
                double[] v = new double[n + 1];

                for (int i = 0; i < j; i++)
                {
                    v[i] = LD[j, i] * LD[i, i];
                }

                double sum = 0.0;
                for (int i = 0; i < j; i++)
                {
                    sum += LD[j, i] * v[i];
                }

                v[j] = LD[j, j] - sum;
                LD[j, j] = v[j];

                for (int i = j + 1; i < n; i++)
                {
                    double subSum = 0.0;
                    for (int k = 0; k < j; k++)
                    {
                        subSum += LD[i, k] * v[k];
                    }
                    LD[i, j] = (LD[i, j] - subSum) / v[j];
                }
            }
        }

        public static double[] Solve(double[,] LDdecomposed, double[] values)
        {
            int n = values.Length;
            double[] X = new double[n];
            for (int i = 1; i < n; i++)
            {
                for (int j = 0; j < i; ++j)
                {
                    values[i] -= LDdecomposed[i, j] * values[j];
                }
            }

            for (int i = 0; i < n; i++)
            {
                values[i] /= LDdecomposed[i, i];
            }

            for (int i = n - 2; i >= 0; i--)
            {
                for (int j = i + 1; j < n; j++)
                {
                    values[i] -= LDdecomposed[j, i] * values[j];
                }
            }

            for (int i = 0; i < n; i++)
            {
                X[i] = values[i];
            }
            return X;
        }
    }
}
