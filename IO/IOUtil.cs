using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace multiprocess_lab4.IO
{
    public class IOUtil
    {
        public static void GenerateSystems(int n = 10)
        {
            CreateDirectory("matrices");
            CreateDirectory("values");

            Random r = new Random();
            for(int s = 0; s < n; s++)
            {
                int size = r.Next(100, 200);
                int[,] matrix = new int[size, size];
                int[] values = new int[size];
                for(int i = 0; i < size; i++)
                {
                    values[i] = r.Next(1, 100);
                    for(int j = 0; j < size; j++)
                    {
                        matrix[i, j] = r.Next(1, 100);
                    }
                }
                using (var matrixWriter = new StreamWriter("matrices/" + s + ".txt"))
                {
                    using (var valuesWriter = new StreamWriter("values/" + s + ".txt")) 
                    {
                        matrixWriter.WriteLine(size + " " + size);
                        valuesWriter.WriteLine(size);
                        for (int i = 0; i < size; i++)
                        {
                            valuesWriter.Write(values[i]);
                            for (int j = 0; j < size; j++)
                            {
                                matrixWriter.Write(matrix[i, j]);
                                if (j + 1 < size)
                                {
                                    matrixWriter.Write(" ");
                                }
                            }
                            if (i + 1 < size)
                            {
                                valuesWriter.Write(" ");
                                matrixWriter.WriteLine();
                            }
                        }
                    }
                }
            }
        }

        public static void WriteArrays(string directory, IEnumerable<double[]> arrays)
        {
            CreateDirectory(directory);
            int i = 0;
            foreach (double[] array in arrays)
            {
                using (var arrayWriter = new StreamWriter(directory + "/" + i + ".txt"))
                {
                    arrayWriter.WriteLine(array.Length);
                    foreach (var value in array)
                    {
                        arrayWriter.Write(value + " ");
                    }
                }
                i++;
            }
        }

        public static IEnumerable<LinearSystem> ReadFromFiles(int n)
        {
            List<double[,]> matrices = new List<double[,]>();
            List<double[]> values = new List<double[]>();

            for(int i = 0; i < n; i++)
            {
                double[] fileValues = File.ReadAllLines("values/" + i + ".txt")
                                          .Skip(1)
                                          .First()
                                          .Trim()
                                          .Split(' ')
                                          .Select(word => double.Parse(word))
                                          .ToArray();

                //double[] fileMatrix = File.ReadAllLines("matrices/" + i + ".txt")
                //                            .Skip(1)
                //                            .SelectMany(line => line.Split(' ')
                //                                                .Select(word => double.Parse(word))
                //                                                .ToArray())
                //                            .ToArray();
                string[] matrixLines = File.ReadAllLines("matrices/" + i + ".txt").Skip(1).ToArray();
                int size = matrixLines.Length;
                double[,] fileMatrix = new double[size, size];
                for (int mi = 0; mi < size; mi++)
                {
                    foreach(var line in matrixLines)
                    {
                        int mj = 0;
                        foreach(var value in line.Trim().Split(' ').Select(w => double.Parse(w))) {
                            fileMatrix[mi, mj] = value;
                            mj++;
                        }
                    }
                }
                matrices.Add(fileMatrix);
                values.Add(fileValues);
            }

            List<LinearSystem> linearSystems = new List<LinearSystem>();
            for(int i = 0; i < matrices.Count; i++)
            {
                linearSystems.Add(new LinearSystem(values[i], matrices[i]));
            }
            return linearSystems;
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
