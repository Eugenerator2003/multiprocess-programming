using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Generator
{
    public class Generator
    {
        public static void GenerateSystems(int n = 10)
        {
            CreateDirectory("matrices");
            CreateDirectory("values");

            Random r = new Random();
            for (int s = 0; s < n; s++)
            {
                int size = r.Next(100, 200);
                int[,] matrix = new int[size, size];
                int[] values = new int[size];
                for (int i = 0; i < size; i++)
                {
                    values[i] = r.Next(1, 100);
                    for (int j = 0; j < size; j++)
                    {
                        matrix[i, j] = r.Next(1, 100);
                    }
                }

                var builder = new StringBuilder();
                builder.AppendLine(size + " " + size);
                for (int i = 0; i < size; i++)
                {
                    for (int j = 0; j < size; j++)
                    {
                        builder.Append(matrix[i, j]);
                        if (j + 1 < size)
                        {
                            builder.Append(' ');
                        }
                    }
                    if (i + 1 < size)
                    {
                        builder.AppendLine();
                    }
                }
                using var matrixWriter = new StreamWriter("matrices/" + s + ".txt");
                Task matrixTask = matrixWriter.WriteAsync(builder.ToString());

                builder.Clear();
                builder.AppendLine(size.ToString());
                for (int i = 0; i < size; i++)
                {
                    builder.Append(values[i]);
                    if (i + 1 < size)
                    {
                        builder.Append(" ");
                    }
                }

                using var valuesWriter = new StreamWriter("values/" + s + ".txt");
                Task valuesTask = valuesWriter.WriteAsync(builder.ToString());

                matrixTask.Wait();
                valuesTask.Wait();
            }
        }

        private static void CreateDirectory(string directory)
        {
            if (!Directory.Exists(directory))
            {
                Directory.CreateDirectory(directory);
            }
        }
    }
}
