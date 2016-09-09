using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace Prototype.Core
{
    public enum GameState
    {
        INTRO, GAME
    }

    public class GameController
    {
        public GameState GameState { get; set; }
        public Point MousePosition { get; set; }
        public List<Key> KeysPressed { get; set; }
        private DateTime lastShot;
        private float actualCadency = 5;
        private float actualSpawn = 0.3f;
        private List<Bullet> bullets = new List<Bullet>();
        private List<Copter> copters = new List<Copter>();
        private List<Parashooter> para = new List<Parashooter>();

        private int actualScore = 0;
        private int actualLives = 15;

        private DateTime lastCopterSpawn;

        private float bulletSpeed = 5;

        public GameController()
        {
            KeysPressed = new List<Key>();
        }

        public bool CanSpawnCopter()
        {
            return ((DateTime.Now - lastCopterSpawn).TotalSeconds > 1.0/actualSpawn);
        }

        public bool CanShoot()
        {
            return ((DateTime.Now - lastShot).TotalSeconds > 1.0/actualCadency);
        }

        public float CanonRotation { get; set; }

        public void Update(TimeSpan delta, TimeSpan total)
        {
            lock (this)
            {
                if (GameState == GameState.INTRO)
                {
                    if (KeysPressed.Any()) GameState = GameState.GAME;
                }
                else
                {
                    if (actualLives <= 0)
                    {
                        Restart();
                        return;
                    }

                    if (KeysPressed.Contains(Key.Left))
                    {
                        if (CanonRotation > -75) CanonRotation -= (float) delta.TotalSeconds*60;
                    }
                    if (KeysPressed.Contains(Key.Right))
                    {
                        if (CanonRotation < 75) CanonRotation += (float) delta.TotalSeconds*60;
                    }
                    if (KeysPressed.Contains(Key.Space))
                    {
                        if (CanShoot()) Shoot();
                    }

                    if (copters.Count < 6 && CanSpawnCopter()) SpawnHelicopter();

                    bullets.ToList().ForEach(UpdateBullet);

                    copters.ToList().ForEach(UpdateCopter);

                    para.ToList().ForEach(UpdatePara);
                }
            }
        }

        private void Restart()
        {
            GameState = GameState.INTRO;
            KeysPressed.Clear();
            bullets.Clear();
            copters.Clear();
            para.Clear();
        }

        private void UpdatePara(Parashooter para)
        {
            if (!para.IsGrounded && para.PosY > 264)
            {
                actualLives--;
                para.IsGrounded = true;
            }
            else if(!para.IsGrounded) para.PosY += (float)Math.Sqrt(para.PosY/50)*0.5f;

            if (para.IsHit && para.HitFrame++ > 10) this.para.Remove(para);
        }

        private void UpdateCopter(Copter copter)
        {
            int next = rnd.Next(0, 60);
            copter.PosX += bulletSpeed*(copter.LeftDirection ? -1 : 1);

            if (copter.IsHit && copter.HitFrame++ > 10) copters.Remove(copter);

            if (copter.PosX < -111 || copter.PosX > 640) copter.LeftDirection = !copter.LeftDirection;

            if (!copter.IsHit && next == 15 && (copter.PosX <= 280 || copter.PosX >= 340) && (copter.PosX > 50 || copter.PosX < 580))
            {
                SpawnParatrooper(copter.PosX+55, copter.PosY+50);
            }
        }

        private void SpawnParatrooper(float posX, float posY)
        {
            Parashooter shooter = new Parashooter(posX, posY);
            para.Add(shooter);
        }

        private Random rnd = new Random();

        private void SpawnHelicopter()
        {
            lastCopterSpawn = DateTime.Now;

            float posY = rnd.Next(0, 50);
            float posX = rnd.NextDouble() < 0.5 ? -111 : 640;

            bool leftdirection = posX == 640;

            Copter copt = new Copter(posX, posY, leftdirection);
            copters.Add(copt);
        }

        private void Shoot()
        {
            float canonLength = 37;
            float posX = (float) (318 + 2 + canonLength*Math.Sin(CanonRotation/180*Math.PI));
            float posY = (float) (223 + (canonLength - canonLength*Math.Cos(CanonRotation/180*Math.PI)));

            float velX = (float) Math.Sin(CanonRotation/180*Math.PI);
            float velY = -(float) Math.Cos(CanonRotation/180*Math.PI);


            var bullet = new Bullet(posX, posY, velX, velY, CanonRotation);
            bullets.Add(bullet);
            lastShot = DateTime.Now;
        }

        public void Draw(Canvas canvas)
        {
            if (GameState == GameState.INTRO)
            {
                DrawImage(canvas, "pack://application:,,,/Images/intro.png", 0, 0, 0, 0, 0);
            }
            else
            {
                DrawImage(canvas, "pack://application:,,,/Images/background.png", 0, 0, 0, 0, 0);
                DrawImage(canvas, "pack://application:,,,/Images/canon.png", 318, 223, CanonRotation, 0.5f, 1);

                bullets.ToList().ForEach(bul => DrawBullet(canvas, bul));
                copters.ToList().ForEach(cop => DrawCopter(canvas, cop));
                para.ToList().ForEach(pr => DrawParashooter(canvas, pr));
            }

            if (MousePosition.X != 0 && MousePosition.Y != 0)
            {
                DrawDebugPoint(canvas, MousePosition.X, MousePosition.Y);
            }

            DrawScoreAndLives(canvas);
        }

        private void DrawParashooter(Canvas canvas, Parashooter shooter)
        {
            if (shooter.IsGrounded)
            {
                DrawImage(canvas, "pack://application:,,,/Images/soldier.png", shooter.PosX, shooter.PosY, 0, 0.0f, 0.0f);

            }
            else
            {
                DrawImage(canvas, "pack://application:,,,/Images/para.png", shooter.PosX, shooter.PosY, 0, 0.0f, 0.0f);
            }

            if (shooter.IsHit) DrawImage(canvas, "pack://application:,,,/Images/explosion2.png", shooter.PosX, shooter.PosY, 0, 0.5f, 0.0f);
        }

        private void DrawCopter(Canvas canvas, Copter copter)
        {
            if (copter.Frame++%10 < 5)
            {
                DrawImage(canvas, "pack://application:,,,/Images/copter1.png", copter.LeftDirection ? copter.PosX : copter.PosX + 111, copter.PosY, 0, 0.0f, 0.0f,
                    !copter.LeftDirection);

            }
            else
            {
                DrawImage(canvas, "pack://application:,,,/Images/copter2.png", copter.LeftDirection ? copter.PosX : copter.PosX+111, copter.PosY, 0, 0.5f, 0.0f,
                    !copter.LeftDirection);

            }

            if (copter.IsHit) DrawImage(canvas, "pack://application:,,,/Images/explosion.png", copter.PosX, copter.PosY, 0, 0.5f, 0.0f);
        }

        private void DrawBullet(Canvas canvas, Bullet bullet)
        {
            DrawImage(canvas, "pack://application:,,,/Images/bullet.png", bullet.PosX, bullet.PosY, bullet.Rotation,
                0.5f, 0.5f);

        }

        private void UpdateBullet(Bullet bullet)
        {
            bullet.PosX += bullet.VelX*bulletSpeed;
            bullet.PosY += bullet.VelY*bulletSpeed;

            if (bullet.PosX < 0 || bullet.PosX > 640 || bullet.PosY < 0 || bullet.PosY > 340) bullets.Remove(bullet);
            else
            {
                foreach (var copter in copters)
                {
                    if (bullet.PosX >= copter.PosX && bullet.PosX <= (copter.PosX + 115) && bullet.PosY >= copter.PosY &&
                        bullet.PosY <= (copter.PosY + 51))
                    {
                        copter.IsHit = true;
                        bullets.Remove(bullet);
                        actualScore += 100;
                        return;
                    }
                }

                foreach (var shooter in para)
                {
                    if (bullet.PosX >= shooter.PosX && bullet.PosX <= (shooter.PosX + 20) && bullet.PosY >= shooter.PosY &&
                      bullet.PosY <= (shooter.PosY + 20))
                    {
                        actualScore += 20;
                        shooter.IsHit = true;
                        bullets.Remove(bullet);
                        return;
                    }
                }
            }
        }

        private void DrawDebugPoint(Canvas canvas, double x, double y)
        {
            TextBlock text = new TextBlock();
            text.Text = "[" + x + "," + y + "]";
            text.Background = Brushes.Red;
            canvas.Children.Add(text);
            Canvas.SetLeft(text, x);
            Canvas.SetTop(text, y);
        }

        private void DrawScoreAndLives(Canvas canvas)
        {
            TextBlock text = new TextBlock();
            text.Text = "Score: "+actualScore.ToString();
            text.FontSize = 20;
            canvas.Children.Add(text);
            Canvas.SetLeft(text, 20);
            Canvas.SetTop(text, 20);

            text = new TextBlock();
            text.Text = "Lives: "+actualLives.ToString();
            text.FontSize = 20;
            canvas.Children.Add(text);
            Canvas.SetLeft(text, 500);
            Canvas.SetTop(text, 20);
        }

        private void DrawImage(Canvas canvas, string path, float x, float y, float rotation, float rotOriginX,
            float rotOriginY, bool scaleInvert = false)
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

            if (rotation != 0)
            {
                var rotate = new RotateTransform(rotation);

                finalImage.RenderTransform = rotate;

                //origin for object
                finalImage.RenderTransformOrigin = new Point(rotOriginX, rotOriginY);
            }

            if (scaleInvert)
            {
                finalImage.RenderTransform = new ScaleTransform(-1, 1);
            }
        }
    }
}
