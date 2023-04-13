using System;
using System.Collections;

namespace NeonShooter
{
    public class FreeList<T>
    {
        append List<T>		elements 		= .();
		append List<int> 	freeElements 	= .();

		public ref T this[int index]
		{
			get { return ref elements[index]; }
		}

		public int Count
		{
			get { return elements.Count; }
		}

		public int Capacity
		{
			get { return elements.Capacity; }
		}

		public void Clear()
		{
			elements.Clear();
			freeElements.Clear();
		}

		public void Collect(int index)
		{
			if (index > -1 && index < Count)
			{
				freeElements.Add(index);
			}
		}

		public void Add(T value)
		{
			if (freeElements.Count > 0)
			{
				int index = freeElements.PopBack();
				elements[index] = value;
			}
			else
			{
				elements.Add(value);
			}
		}

		public void Remove(T value)
		{
			int index = elements.IndexOf(value);
			Collect(index);
		}
    }
}