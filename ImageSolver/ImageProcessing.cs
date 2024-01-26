using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static System.Net.Mime.MediaTypeNames;

namespace SmoothSolver
{
    internal class ImageProcessing
    {
        public static byte[,,] GetWindow(Bitmap bitmap, int iPixel, int jPixel, int r)
        {
            int imageHeight = bitmap.Height;
            int imageWidth = bitmap.Width;

            byte[,,] window = new byte[r, r, 3];

            for (int i = 0; i < r; i++)
            {
                for (int j = 0; j < r; j++)
                {
                    int rowIndex = Math.Min(Math.Max(0, iPixel - r / 2 + i), imageHeight - 1);
                    int colIndex = Math.Min(Math.Max(0, jPixel - r / 2 + j), imageWidth - 1);

                    Color color = bitmap.GetPixel(colIndex, rowIndex);
                    window[i, j, 0] = color.R;
                    window[i, j, 1] = color.G;
                    window[i, j, 2] = color.B;
                }
            }

            return window;
        }
    }
}
