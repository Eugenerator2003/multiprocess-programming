using System.Drawing;
using System.IO.Pipes;
using SmoothSolver;

namespace SegmentationSolver
{
    internal class Program
    {
        static void Main()
        {
            Segmentator segmentator = new Segmentator(6);

            Thread.Sleep(100);
            Directory.SetCurrentDirectory(@"f:/laba5/Processed");
            if (!Directory.Exists(@"f:/laba5/Segmented"))
            {
                Directory.CreateDirectory(@"f:/laba5/Segmented");
            }

            using (NamedPipeClientStream pipeClient = new NamedPipeClientStream(".", "laba5", PipeDirection.In))
            {
                pipeClient.Connect();

                Console.WriteLine("Connected to named pipe laba5");
                using (StreamReader sr = new StreamReader(pipeClient))
                {
                    string message;
                    while (true)
                    {
                        message = sr.ReadLine();
                        if (message != null)
                        {
                            Console.WriteLine($"Message from pipe: {message}");
                            if (message != "ALL")
                            {
                                Bitmap bitmap = new Bitmap(message);
                                // path for image: message
                                Console.WriteLine("Start segmenting");
                                segmentator.Segment(bitmap);
                                // work
                                // path for segmentation: "../Segmented/" + message
                                bitmap.Save("../Segmented/" + message);
                                Console.WriteLine("Segmented");
                            }
                            else
                            {
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
            Console.Write("Press Enter to continue...");
            Console.ReadLine();
        }
    }
}