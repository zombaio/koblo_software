// Copyright 2004, 2005,2006,2007,2008 Koblo http://koblo.com
//
// This file is part of the Koblo SDK.
//
// the Koblo SDK is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// the Koblo SDK is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with the Koblo Stools. If not, see <http://www.gnu.org/licenses/>.



/*! \class CWindowOSXQT
 * \brief QuickTime specific implementation of IWindow
*/
class CWindowOSXQT : public virtual CWindowOSX
{
public:
	//! Constructor
	CWindowOSXQT();
	//! Destructor
	virtual ~CWindowOSXQT();

	//! IDestructable override
	virtual void Destroy();

	//! IWindow override
	virtual void GetHDC(void** pp);
	//! IWindow override
	virtual void GetDrawPointer(void** pp);
	//! IWindow override
	virtual void PaintToBuffer(tuchar* puchBuffer, tuint32 uiInterleave, const SRect& rUpdate);

	//! CWindow override
	virtual void Paint();

protected:
	//! Pointer to drawing buffer
	tuchar* mpcDrawBuffer;

	//! Size of currently allocated drawing buffer size
	tint miDrawBufferSize;

	//! Drawing message action
	virtual void OnPaint();
};



