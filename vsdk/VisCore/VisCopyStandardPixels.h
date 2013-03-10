// VisCopyStandardPixels.h
//
// Copyright © 1997-2000 Microsoft Corporation, All Rights Reserved


VisCoreExport bool VISAPI VisCopyStandardPixels(int bandSrc, int bandDest,
		const CVisImageBase& refimageSrc, CVisImageBase& refimageDest,
		const RECT& refrectSrc, const POINT pointDest,
		EVisNormalize evisnormalize = evisnormalizeDefault,
		double dblMin = 1.0, double dblMax = - 1.0);
