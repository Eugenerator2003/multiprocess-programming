using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace multiprocess_lab4.IO
{
    public class IOUtil
    {
        public static void WriteArrays(string directory, IEnumerable<double[]> arrays)
        {
            CreateDirectory(directory);
            int i = 0;
            foreach (double[] array in arrays)
            {
                var builder = new StringBuilder(array.Length.ToString() + "\n");
                foreach (var value in array)
                {
                    builder.Append(value + " ");
                }
                using var arrayWriter = new StreamWriter(directory + "/" + i + ".txt");
                arrayWriter.WriteAsync(builder.ToString());
                i++;
            }
        }

        public static IEnumerable<LinearSystem> ReadFromFiles(int n)
        {
            List<double[,]> matrices = new List<double[,]>();
            List<double[]> values = new List<double[]>();

            for(int i = 0; i < n; i++)
            {
                using var valuesReader = new StreamReader("values/" + i + ".txt");
                var taskValues = valuesReader.ReadToEndAsync();
                using var matrixReader = new StreamReader("matrices/" + i + ".txt");
                var taskMatrix = matrixReader.ReadToEndAsync();

                double[] fileValues = taskValues.Result.Split('\n')
                                                       .Skip(1)
                                                       .First()
                                                       .Split(' ')
                                                       .Select(w => double.Parse(w.Trim()))
                                                       .ToArray();

                string[] matrixLines = taskMatrix.Result.Split('\n').Skip(1).ToArray();
                int size = matrixLines.Length;
                double[,] fileMatrix = new double[size, size];
                for(int im = 0; im < size; im++)
                {
                    int jm = 0;
                    foreach(var value in matrixLines[im].Split(' ').Select(w => double.Parse(w.Trim())))
                    {
                        fileMatrix[im, jm] = value;
                        jm++;
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

        public static void CreateDirectory(string directory)
        {
            if (!Directory.Exists(directory))
            {
                Directory.CreateDirectory(directory);
            }
        }
    }
}
