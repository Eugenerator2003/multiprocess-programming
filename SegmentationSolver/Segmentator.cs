using System;
using System.Collections.Generic;
using System.Linq;
using System.Drawing;
using System.Text;
using System.Threading.Tasks;
using SmoothSolver;

namespace SegmentationSolver
{
    public class Segmentator
    {
        public int K { get; set; }

        public Segmentator(int k)
        {
            K = k;
        }

        public void Segment(Bitmap bitmap)
        {
            var pixels = GetPixels(bitmap);
            var labels = KMeans(pixels, K);
            ApplyClusters(bitmap, pixels, labels);
        }

        private List<ProcessedPixel> GetPixels(Bitmap bitmap)
        {
            List<ProcessedPixel> data = new List<ProcessedPixel>();
            for (int i = 0; i < bitmap.Height; i++)
            {
                for (int j = 0; j < bitmap.Width; j++)
                {
                    var pixel = new ProcessedPixel()
                    {
                        Color = bitmap.GetPixel(j, i),
                        Row = i,
                        Column = j
                    };
                    data.Add(pixel);
                }
            }
            return data;
        }

        private int[] KMeans(List<ProcessedPixel> data, int k, int maxIterations = 100)
        {
            ProcessedPixel[] centroids = new ProcessedPixel[k];
            int step = data.Count() / k;
            for(int i = 0; i < k; i++)
            {
                centroids[i] = data[step * i];
            }

            int[] labels = null;
            var comparer = new PixelUtil.Comparer();
            for (int iteration = 0; iteration < maxIterations; iteration++)
            {
                labels = data.Select(point => Array.IndexOf(centroids, centroids
                                                                       .OrderBy(centroid => PixelUtil
                                                                                             .Distance(point, centroid))
                                                                       .First()))
                            .ToArray();

                ProcessedPixel[] newCentroids = Enumerable
                                    .Range(0, k)
                                    .Select(i => {
                                                var red = (int)data.Where((_, index) => labels[index] == i).Average(p => p.Color.R);
                                                var green = (int)data.Where((_, index) => labels[index] == i).Average(p => p.Color.G);
                                                var blue = (int)data.Where((_, index) => labels[index] == i).Average(p => p.Color.B);
                                                var pixel = new ProcessedPixel()
                                                {
                                                    Color = Color.FromArgb(red, green, blue)
                                                };
                                                return pixel;
                                            })
                                    .ToArray();

                if (centroids.SequenceEqual(newCentroids, comparer))
                    break;

                centroids = newCentroids;
            }

            return labels;
        }

        private void ApplyClusters(Bitmap bitmap, List<ProcessedPixel> data, int[] clusterLabels)
        {
            var unique = clusterLabels.Distinct();
            var colorDict = new Dictionary<int, Color>();
            var random = new Random();
            foreach (var label in unique)
            {
                colorDict.Add(label, Color.FromArgb(random.Next(0, 256), random.Next(0, 256), random.Next(0, 256)));
            }

            for (int i = 0; i < data.Count; i++)
            {
                bitmap.SetPixel(data[i].Column, data[i].Row, colorDict[clusterLabels[i]]);
            }
        }
    }
}
