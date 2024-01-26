using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmoothSolver
{
    public class ImageWindow
    {
        public byte[,,] Window { get; }

        public int Row { get; }

        public int Column { get; }

        public ImageWindow(byte[,,] window, int row, int column)
        {
            Window = window;
            Row = row;
            Column = column;
        }
    }
}
