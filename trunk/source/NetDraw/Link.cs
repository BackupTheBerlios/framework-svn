using System;
using System.Drawing;

namespace NetDraw
{
	/// <summary>
	/// Summary description for Link.
	/// </summary>
	public class Link
	{
		public Link(int s, int t, BreadQueue br)
		{
			if (s > t)
			{	
				source = t;
				target = s;
			}
			else
			{
				source = s;
				target = t;
			}
			boundRectangle = new RectangleF(Math.Min(br.PointList.Locations[source].X, 
													br.PointList.Locations[target].X),
										Math.Min(br.PointList.Locations[source].Y, 
												 br.PointList.Locations[target].Y),
				Math.Abs(br.PointList.Locations[s].X - br.PointList.Locations[t].Y), 
				Math.Abs(br.PointList.Locations[s].X - br.PointList.Locations[t].Y)); 
			key = ((ulong) source) << 32 | (ulong) target;
		}
		public RectangleF boundRectangle;
		public long generation;
		public long born;

		public int source;
		public int target;
		public float percentage = 0;
		public ulong key;
	}
}
