NDSummary.OnToolTipsLoaded("File:sd.c",{94:"<div class=\"NDToolTip TFile LC\"><div class=\"TTSummary\">SD card functions.</div></div>",96:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype96\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">unsigned char</span> sd_init(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"first\"></td><td class=\"PType\">CID&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">cid,</td></tr><tr><td class=\"first\"></td><td class=\"PType\">CSD&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">csd,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">unsigned</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">long</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">capacity</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Initialise the SD card and returns card CID, CSD and capacity.</div></div>",97:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype97\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">unsigned char</span> sd_block_read(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">unsigned</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">long</span>&nbsp;</td><td></td><td class=\"PName last\">addr,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">unsigned</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">char</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">buffer</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Reads a sector of data from sector addr.</div></div>",98:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype98\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">unsigned char</span> sd_block_write(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">unsigned</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">long</span>&nbsp;</td><td></td><td class=\"PName last\">addr,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">unsigned</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">char</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">buffer</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Writes a sector of data at sector addr.</div></div>",99:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype99\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> sd_stream_open(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">unsigned</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">long</span>&nbsp;</td><td class=\"PName last\">addr</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Start a stream write at the specified address (used both for caching and non-caching streaming writes).</div></div>",100:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype100\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">unsigned char</span> sd_stream_write(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">unsigned</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">char</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">buffer,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">unsigned</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">short</span>&nbsp;</td><td></td><td class=\"PName last\">size,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">unsigned</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">long</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">currentsect</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Writes data in streaming multiblock write</div></div>",101:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype101\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">unsigned char</span> sd_streamcache_write(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">unsigned</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">char</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">buffer,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">unsigned</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">short</span>&nbsp;</td><td></td><td class=\"PName last\">size,</td></tr><tr><td class=\"PModifierQualifier first\"><span class=\"SHKeyword\">unsigned</span>&nbsp;</td><td class=\"PType\"><span class=\"SHKeyword\">long</span>&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">currentsect</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Writes data in streaming multiblock write with caching.</div></div>",102:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype102\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> sd_print_csd(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">FILE&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">f,</td></tr><tr><td class=\"PType first\">CSD&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">csd</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Prints the CSD fields to a stream.</div></div>",103:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype103\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> sd_print_cid(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">FILE&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">f,</td></tr><tr><td class=\"PType first\">CID&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">cid</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Prints the CID fields to a stream.</div></div>",104:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype104\" class=\"NDPrototype WideForm CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> sd_print_ocr(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">FILE&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">f,</td></tr><tr><td class=\"PType first\">OCR&nbsp;</td><td class=\"PNamePrefix\">*</td><td class=\"PName last\">ocr</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div><div class=\"TTSummary\">Prints the OCR fields to a stream.</div></div>"});