using System;
using System.Drawing;
using System.Collections;

namespace NetDraw
{
	/// <summary>
	/// Summary description for BreadQueue.
	/// </summary>
	public class BreadQueue
	{
		public bool useRect;
		public PointList PointList;
		SortedList allStableLinks = new SortedList();
		SortedList allDyingLinks = new SortedList();
		SortedList allBornLinks = new SortedList();
		public float zoom = 1;
		public float drawWidth;
		public float drawHeight;
		
		public float minX;
		public float minY;
		public float maxX;
		public float maxY;
		public BreadQueue()
		{

		}
		long gen = 1;
		public void EndGeneration()
		{
			KillOldOnes();
			gen++;
		}
		public void KillOldOnes()
		{
			for (int n = allStableLinks.Count-1; n >= 0; n--)
			{
				Link l = (Link) allStableLinks.GetByIndex(n);
				if (l.generation != gen)
				{
					allStableLinks.RemoveAt(n);
					allDyingLinks.Add(l.key,l);
				}
			}
			for (int n = allBornLinks.Count-1; n >= 0; n--)
			{
				Link l = (Link) allBornLinks.GetByIndex(n);
				if (l.generation != gen)
				{
					allBornLinks.RemoveAt(n);
					allDyingLinks.Add(l.key, l);
				}
			}
		}
		public void Feed(Link []linkList)
		{
			foreach (Link link in linkList)
			{
				ulong key = link.key;
				Link refLink;
				refLink = (Link) allStableLinks[key];
				if (refLink == null)
				{	
					// se fija si está muriendo...
					refLink = (Link) allDyingLinks[key];
					if (refLink != null)
					{	// lo revive...
						allBornLinks.Add(key, refLink);
						allDyingLinks.Remove(key);
					}
					else
					{
						// no está ni estable ni muriendo ni naciendo...
						refLink = (Link) allBornLinks[key];
						if (refLink == null)
						{	// si no está naciendo, lo tiene que hacer nacer...
							refLink = link;
							allBornLinks.Add(key, link);
						}
					}
				}
				refLink.generation = gen;
			}
		}
		ArrayList ModifiedRectangles = new ArrayList();
		public void Draw(Graphics g, bool FirstTime)
		{
			//g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
			// Limpia
			// Dibuja las líneas
			if (useRect) ModifiedRectangles.Clear();
			foreach (Link l in allDyingLinks.Values)
			{
				if (useRect) ModifiedRectangles.Add(l.boundRectangle);
				DrawLink(g, l, Pens.Red);
			}
			foreach (Link l in allBornLinks.Values)
			{
				if (useRect) ModifiedRectangles.Add(l.boundRectangle);
				DrawLink(g, l, Pens.Green);
			}
			foreach (Link l in allStableLinks.Values)
				if (useRect == false || FirstTime || IsInModifiedRectangles(l.boundRectangle))
				{
					DrawLink(g, l, Pens.Black);
					if (useRect) ModifiedRectangles.Add(l.boundRectangle);
				}
			// Dibuja encima los puntos
			foreach(PointF p in this.PointList.Locations)
			{
				//g.FillEllipse(Brushes.Blue, p.X-r/2, p.Y-r/2, r,r);
				//g.FillEllipse(Brushes.White, p.X-r2/2, p.Y-r2/2, r2,r2);
				if (useRect == false || FirstTime || IsInModifiedRectangles(new RectangleF(p.X-1, p.Y-1, 3,3)))
				{
					g.DrawRectangle(Pens.Blue, TranslateX(p.X) * zoom -1, TranslateY(p.Y) * zoom-1, 2,2);
					g.DrawRectangle(Pens.White, TranslateX(p.X) * zoom, TranslateY(p.Y) * zoom, 0,0);
				}
			}

		}
		public bool IsInModifiedRectangles(RectangleF r)
		{
			foreach(RectangleF rr in ModifiedRectangles)
				if (rr.IntersectsWith(r)) return true;
			return false;
		}
		public void DrawLink(Graphics g, Link l, Pen p)
		{
			if (l.percentage >= 1)
				g.DrawLine(Pens.Black, zoom * TranslateX(PointList.Locations[l.source].X), zoom *TranslateY(PointList.Locations[l.source].Y), zoom *TranslateX(PointList.Locations[l.target].X), zoom *TranslateY(PointList.Locations[l.target].Y));
			else
			{
				// Dibuja parcial
				PointF a = TranslatePoint(PointList.Locations[l.source]);
				PointF b = TranslatePoint(PointList.Locations[l.target]);
				float offsetx = a.X - b.X;
				float offsety = a.Y - b.Y;
				float perc = Math.Max(0, Math.Min(1, l.percentage));
				PointF a2 = new PointF((a.X - offsetx * perc * .5F), (a.Y - offsety * l.percentage * .5F));
				PointF b2 = new PointF((b.X + offsetx * perc * .5F), (b.Y + offsety * l.percentage * .5F));
				g.DrawLine(p, zoom * a.X, zoom * a.Y , zoom *a2.X, zoom * a2.Y);
				g.DrawLine(p, zoom * b.X, zoom * b.Y , zoom *b2.X, zoom * b2.Y);
			}
		}
		private PointF TranslatePoint(PointF p)
		{
			return new PointF(TranslateX(p.X), TranslateY(p.Y));
		}
		private float TranslateX(float x)
		{
			return drawWidth * (x - minX) / (maxX - minX);
		}
		private float TranslateY(float y)
		{
			return drawHeight * (y - minY) / (maxY - minY);
		}

		public void Evolve(float stepsize)
		{
			for (int n = allDyingLinks.Count-1; n >= 0; n--)
			{
				Link l = (Link) allDyingLinks.GetByIndex(n);
				if (l.percentage <= 0)
				{	// RIP (desaparece del mundo)...
					l.percentage = 0;
					allDyingLinks.RemoveAt(n);
				}
				l.percentage -= stepsize;
			}
			for (int n = allBornLinks.Count-1; n >= 0; n--)
			{
				Link l = (Link) allBornLinks.GetByIndex(n);
				if (l.percentage >= 1)
				{
					l.percentage = 1;
					allBornLinks.RemoveAt(n);
					allStableLinks.Add(l.key, l);
				}
				l.percentage += stepsize;
			}
			
		}
	}
}
