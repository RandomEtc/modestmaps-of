/*
 *  QueueSorter.h
 *  modestMapsExample
 *
 *  Created by Tom Carden on 8/21/10.
 *  Copyright 2010 Stamen Design. All rights reserved.
 *
 */

// TODO: implement this and use if in Map.h
/*
public class QueueSorter implements Comparator {
    Coordinate center;
    public void setCenter(Coordinate center) {
		this.center = center;
    } 
    public int compare(Object o1, Object o2) {
		Coordinate c1 = (Coordinate)o1; 
		Coordinate c2 = (Coordinate)o2;
		if (c1.zoom == center.zoom) {
			if (c2.zoom == center.zoom) {
				float d1 = p.dist(center.column, center.row, c1.column, c1.row);
				float d2 = p.dist(center.column, center.row, c2.column, c2.row);
				return d1 < d2 ? -1 : d1 > d2 ? 1 : 0;
			} 
			else {
				return -1;
			}
		}
		else if (c2.zoom == center.zoom) {
			return 1;
		}
		else {
			float d1 = p.abs(c1.zoom - center.zoom);
			float d2 = p.abs(c2.zoom - center.zoom);
			return  d1 < d2 ? -1 : d1 > d2 ? 1 : 0;
		}
    }
}
*/
