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
        public static void DrawImage(Canvas canvas, string path, float x, float y, float rotation, float rotOriginX,
    float rotOriginY, int zIndex, float scaleX, float scaleY)
        {
            var finalImage = new Image();
            var logo = new BitmapImage();
            logo.BeginInit();
            logo.UriSource = new Uri(path);
            logo.EndInit();
            finalImage.Source = logo;

            canvas.Children.Add(finalImage);
            Canvas.SetLeft(finalImage, x);
            Canvas.SetTop(finalImage, y);
            Canvas.SetZIndex(finalImage,zIndex);

            if (rotation != 0)
            {
                var rotate = new RotateTransform(rotation);

                finalImage.RenderTransform = rotate;

                //origin for object
                finalImage.RenderTransformOrigin = new Point(rotOriginX, rotOriginY);
            }

            if (scaleX != 1 || scaleY != 1)
            {
                finalImage.RenderTransform = new ScaleTransform(scaleX, scaleY);
            }
        }
    }
}
