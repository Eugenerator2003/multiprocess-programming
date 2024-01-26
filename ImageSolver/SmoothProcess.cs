using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmoothSolver
{
    public class SmoothProcess
    {
        public static byte[] ComputeWindow(byte[,,] window)
        {
            byte[] rgb = new byte[3];
            for (int i = 0; i < rgb.Length; i++)
            {
                rgb[i] = ComputeAverage(window, i);
            }
            return rgb;
        }

        public static byte ComputeAverage(byte[,,] window, int channelIndex)
        {
            int sum = 0;
            for (int i = 0; i < window.GetLength(0); i++)
            {
                for (int j = 0; j < window.GetLength(1); j++)
                {
                    sum += window[i, j, channelIndex];
                }
            }

            return (byte)(sum / (window.GetLength(0) * window.GetLength(1)));
        }

        public int R { get; }

        public int Threads { get; }

        private Semaphore semaphore;
        private Queue<ProcessedPixel> pixels;

        public SmoothProcess(int r, int threads)
        {
            R = r;
            Threads = threads;
            semaphore = new Semaphore(1, 1);
            ThreadPool.SetMaxThreads(Threads, Threads);
            pixels = new();
        }

        public void Process(Bitmap bitmap)
        {
            int width = bitmap.Width;
            int height = bitmap.Height;

            for(int i = 0; i < height; i++)
            {
                for(int j = 0; j < width; j++)
                {
                    byte[,,] windowColors = ImageProcessing.GetWindow(bitmap, i, j, R);
                    var window = new ImageWindow(windowColors, i, j);
                    ThreadPool.QueueUserWorkItem(DoAverage, window);
                }
            }

            int taskCount = width * height;
            while (taskCount != pixels.Count()) ;

            for(int i = 0; i < taskCount; i++)
            {
                var pixel = pixels.Dequeue();
                bitmap.SetPixel(pixel.Column, pixel.Row, pixel.Color);
            }
        }

        private void DoAverage(object? objParam)
        {
            var window = objParam as ImageWindow;
            var colors = ComputeWindow(window.Window);
            var pixel = new ProcessedPixel()
            {
                Color = Color.FromArgb(colors[0], colors[1], colors[2]),
                Row = window.Row,
                Column = window.Column,
            };
            semaphore.WaitOne();
            pixels.Enqueue(pixel);
            semaphore.Release();
        }
    }
}
