using SmoothSolver;
using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SegmentationSolver
{
    internal class PixelUtil
    {
        public class Comparer : IEqualityComparer<ProcessedPixel>
        {
            public bool Equals(ProcessedPixel? x, ProcessedPixel? y)
            {
                if (x == null || y == null)
                    return false;

                return x.Color.R == y.Color.R &&
                       x.Color.G == y.Color.G &&
                       x.Color.B == y.Color.B;
            }

            public int GetHashCode([DisallowNull] ProcessedPixel obj)
            {
                return obj.GetHashCode();
            }
        }

        public static double Distance(ProcessedPixel p1, ProcessedPixel p2)
        {
            double dr = p1.Color.R - p2.Color.R;
            double dg = p1.Color.G - p2.Color.G;
            double db = p1.Color.B - p2.Color.B;
            return Math.Sqrt(dr * dr + dg * dg + db * db);
        }
    }
}
