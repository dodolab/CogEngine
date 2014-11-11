using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace CopterDown.Utils
{
    public static class Helper
    {
        public static void DrawImage(Canvas canvas, string path, Matrix matrix, int zIndex)
        {
            var finalImage = new Image();
            var logo = new BitmapImage();
            logo.BeginInit();
            logo.UriSource = new Uri(path);
            logo.EndInit();
            finalImage.Source = logo;

            canvas.Children.Add(finalImage);
            Canvas.SetZIndex(finalImage,zIndex);


            finalImage.RenderTransform = new MatrixTransform(matrix);
        }
    }
}
