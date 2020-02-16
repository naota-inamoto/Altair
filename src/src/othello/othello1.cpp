
#include "stdafx.h"

#ifndef	AL_EXT_API

#ifdef _WINDOWS
#define AL_EXT_API __declspec(dllexport)
#endif
#ifdef _LINUX
#ifdef _MAC
#define AL_EXT_API __attribute__((visibility("default")))
#else
#define AL_EXT_API
#endif
#endif
#endif

#define BORD_SIZE 100

extern "C" long IsMeetSpace(char* Bord, long pos) {
	for (long i = -10; i <= 10; i += 10) {
		for (long j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) continue;
			long c = Bord[pos + i + j];
			if (c == '0' || c == '1') return 1;
		}
	}
	return 0;
}

extern "C" void SpaceWhichMeetPiece(char* Bord, long pos) {
	for (long i = -10; i <= 10; i += 10) {
		for (long j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) continue;
			long p = pos + i + j;
			long c = Bord[p];
			if (c == '0') {
				Bord[p] = '1';
			}
		}
	}
}

extern "C" AL_EXT_API long Play(char* Bord, long Turn, long i, long j) {
	long o, chg = 0;
	long c, p0 = i + j;
	for (long i = -10; i <= 10; i += 10) {
		for (long j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) continue;
			long p = p0;
			long d = i + j;
			o = 0;
			// Number of catched pieces
			for (;;) {
				p = p + d;
				c = Bord[p];
				if (Turn == 0) {
					if (c == '4' || c == '5') {
						o = 1;
					} else {
						break;
					}
				} else {
					if (c == '2' || c == '3') {
						o = 1;
					} else {
						break;
					}
				}
			}
			// does catch adversary's pieces ?
			if (o) {
				if (Turn == 0) {
					if (c == '2' || c == '3') {
					} else {
						o = 0;
					}
				} else {
					if (c == '4' || c == '5') {
					} else {
						o = 0;
					}
				}
			}
			// Play
			if (o) {
				if (chg) {
				} else {
					chg = 1;
					// A Piece which is put
					if (Turn == 0) {
						if (IsMeetSpace(Bord, p0)) {
							Bord[p0] = '2';
						} else {
							Bord[p0] = '3';
						}
					} else {
						if (IsMeetSpace(Bord, p0)) {
							Bord[p0] = '4';
						} else {
							Bord[p0] = '5';
						}
					}
					SpaceWhichMeetPiece(Bord, p0);
				}
				// Reverse put pieces
				for (;;) {
					p = p - d;
					if (p == p0) break;
					if (Turn == 0) {
						if (c == '5') {
							Bord[p] = '3';
						} else {
							if (IsMeetSpace(Bord, p)) {
								Bord[p] = '2';
							} else {
								Bord[p] = '3';
							}
						}
					} else {
						if (c == '3') {
							Bord[p] = '5';
						} else {
							if (IsMeetSpace(Bord, p)) {
								Bord[p] = '4';
							} else {
								Bord[p] = '5';
							}
						}
					}
				}
			}
		}
	}
	return chg;
}

extern "C" long v11cont(char* Bord, long p, long c1, long c2, long d) {
	long num = 0;
	for (p += d;; p += d) {
		long c = Bord[p];
		if (c == c1 || c == c2) {
			num += 40;
		} else {
			return num;
		}
	}
}

extern "C" long BlackMinusWhiteWhichMeetSpace(char* Bord) {
	long c, num = 0;
	for (long i = 20; i <= 70; i += 10) {
		for (long j = 2; j <= 7; j++) {
			c = Bord[i + j];
			if (c == '2') {
				num = num + 1;
			} else if (c == '4') {
				num = num - 1;
			}
		}
	}
	// Adjustment of Corner
	long v11 = 1000;
	long v22 = 200;
	long v12 = 100;
	c = Bord[11];
	if (c == '2' || c == '3') {
		num = num - v11;
		num -= v11cont(Bord, 11, '2', '3', 1);
		num -= v11cont(Bord, 11, '2', '3', 10);
	} else if (c == '4' || c == '5') {
		num = num + v11;
		num += v11cont(Bord, 11, '4', '5', 1);
		num += v11cont(Bord, 11, '4', '5', 10);
	} else {
		c = Bord[22];
		if (c == '2' || c == '3') {
			num = num + v22;
		} else if (c == '4' || c == '5') {
			num = num - v22;
		}
		c = Bord[12];
		if (c == '2' || c == '3') {
			num = num + v12;
		} else if (c == '4' || c == '5') {
			num = num - v12;
		}
		c = Bord[21];
		if (c == '2' || c == '3') {
			num = num + v12;
		} else if (c == '4' || c == '5') {
			num = num - v12;
		}
	}
	c = Bord[18];
	if (c == '2' || c == '3') {
		num = num - v11;
		num -= v11cont(Bord, 18, '2', '3', -1);
		num -= v11cont(Bord, 18, '2', '3', 10);
	} else if (c == '4' || c == '5') {
		num = num + v11;
		num += v11cont(Bord, 18, '4', '5', -1);
		num += v11cont(Bord, 18, '4', '5', 10);
	} else {
		c = Bord[27];
		if (c == '2' || c == '3') {
			num = num + v22;
		} else if (c == '4' || c == '5') {
			num = num - v22;
		}
		c = Bord[17];
		if (c == '2' || c == '3') {
			num = num + v12;
		} else if (c == '4' || c == '5') {
			num = num - v12;
		}
		c = Bord[28];
		if (c == '2' || c == '3') {
			num = num + v12;
		} else if (c == '4' || c == '5') {
			num = num - v12;
		}
	}
	c = Bord[81];
	if (c == '2' || c == '3') {
		num = num - v11;
		num -= v11cont(Bord, 81, '2', '3', 1);
		num -= v11cont(Bord, 81, '2', '3', -10);
	} else if (c == '4' || c == '5') {
		num = num + v11;
		num += v11cont(Bord, 81, '4', '5', 1);
		num += v11cont(Bord, 81, '4', '5', -10);
	} else {
		c = Bord[72];
		if (c == '2' || c == '3') {
			num = num + v22;
		} else if (c == '4' || c == '5') {
			num = num - v22;
		}
		c = Bord[71];
		if (c == '2' || c == '3') {
			num = num + v12;
		} else if (c == '4' || c == '5') {
			num = num - v12;
		}
		c = Bord[82];
		if (c == '2' || c == '3') {
			num = num + v12;
		} else if (c == '4' || c == '5') {
			num = num - v12;
		}
	}
	c = Bord[88];
	if (c == '2' || c == '3') {
		num = num - v11;
		num -= v11cont(Bord, 88, '2', '3', -1);
		num -= v11cont(Bord, 88, '2', '3', -10);
	} else if (c == '4' || c == '5') {
		num = num + v11;
		num += v11cont(Bord, 88, '4', '5', -1);
		num += v11cont(Bord, 88, '4', '5', -10);
	} else {
		c = Bord[77];
		if (c == '2' || c == '3') {
			num = num + v22;
		} else if (c == '4' || c == '5') {
			num = num - v22;
		}
		c = Bord[78];
		if (c == '2' || c == '3') {
			num = num + v12;
		} else if (c == '4' || c == '5') {
			num = num - v12;
		}
		c = Bord[87];
		if (c == '2' || c == '3') {
			num = num + v12;
		} else if (c == '4' || c == '5') {
			num = num - v12;
		}
	}
	return num;
}

extern "C" AL_EXT_API void PeruseNdepth(char* Bord, long Turn, long depth, long CutValue,
												   long man, long* Eval, long* i_max, long* j_max) {
	long Evaluation;
	if (depth <= 0) {
		Evaluation = BlackMinusWhiteWhichMeetSpace(Bord);
		if (man) {
			*Eval = Evaluation; *i_max = 0; *j_max = 0; return;
		} else {
			*Eval = -Evaluation; *i_max = 0; *j_max = 0; return;
		}
	}
	long TurnOfComputer = 0;
	if (man) {
		if (Turn == 0) {
		} else {
			TurnOfComputer = 1;
		}
	} else {
		if (Turn == 0) {
			TurnOfComputer = 1;
		} else {
		}
	}
	if (TurnOfComputer) {
		*Eval =  -30000;
		for (long i = 10; i <= 80; i += 10) {
			for (long j = 1; j <= 8; j++) {
				long c = Bord[i + j];
				if (c == '1') {
					char* NextBord = (char*)malloc(BORD_SIZE);
					memcpy(NextBord, Bord, BORD_SIZE);
					long i_max2, j_max2;
					if (Play(NextBord, Turn, i, j)) {
						PeruseNdepth(NextBord, 1 - Turn, depth - 1, *Eval, man, &Evaluation, &i_max2, &j_max2);
						free(NextBord);
						if (Evaluation > *Eval) {
							*Eval = Evaluation;
							*i_max = i;
							*j_max = j;
						}
						if (Evaluation > CutValue) return;
					} else {
						free(NextBord);
					}
				}
			}
		}
		return;
	} else {
		*Eval = 30000;
		for (long i = 10; i <= 80; i += 10) {
			for (long j = 1; j <= 8; j++) {
				long c = Bord[i + j];
				if (c == '1') {
					char* NextBord = (char*)malloc(BORD_SIZE);
					memcpy(NextBord, Bord, BORD_SIZE);
					long i_max2, j_max2;
					if (Play(NextBord, Turn, i, j)) {
						PeruseNdepth(NextBord, 1 - Turn, depth - 1, *Eval, man, &Evaluation, &i_max2, &j_max2);
						free(NextBord);
						if (Evaluation < *Eval) {
							*Eval = Evaluation;
							*i_max = i;
							*j_max = j;
						}
						if (Evaluation < CutValue) return;
					} else {
						free(NextBord);
					}
				}
			}
		}
		return;
	}
}

extern "C" long NumOfBlackMinusNumOfWhite(char* Bord) {
	long num = 0;
	for (long i = 10; i <= 80; i += 10) {
		for (long j = 1; j <= 8; j++) {
			long c = Bord[i + j];
			if (c == '2' || c == '3') {
				num++;
			} else if (c == '4' || c == '5') {
				num--;
			}
		}
	}
	return num;
}

extern "C" AL_EXT_API void PeruseAll(char* Bord, long Turn, long CutValue, long Pass,
												long man, long* Eval, long* i_max, long* j_max) {
	long Evaluation;
	long TurnOfComputer = 0;
	if (man) {
		if (Turn == 0) {
		} else {
			TurnOfComputer = 1;
		}
	} else {
		if (Turn == 0) {
			TurnOfComputer = 1;
		} else {
		}
	}
	if (TurnOfComputer) {
		*Eval =  -30000;
		for (long i = 10; i <= 80; i += 10) {
			for (long j = 1; j <= 8; j++) {
				long c = Bord[i + j];
				if (c == '1') {
					char* NextBord = (char*)malloc(BORD_SIZE);
					memcpy(NextBord, Bord, BORD_SIZE);
					long i_max2, j_max2;
					if (Play(NextBord, Turn, i, j)) {
						PeruseAll(NextBord, 1 - Turn, *Eval, 0, man, &Evaluation, &i_max2, &j_max2);
						free(NextBord);
						if (Evaluation > *Eval) {
							*Eval = Evaluation;
							*i_max = i;
							*j_max = j;
						}
						if (Evaluation > CutValue) return;
					} else {
						free(NextBord);
					}
				}
			}
		}
		if (*Eval ==  -30000) {
			if (Pass) {
				if (man) {
					*Eval = -NumOfBlackMinusNumOfWhite(Bord);
				} else {
					*Eval = NumOfBlackMinusNumOfWhite(Bord);
				}
			} else {
				long i_max2, j_max2;
				PeruseAll(Bord, 1 - Turn, *Eval, 1, man, Eval, &i_max2, &j_max2);
			}
		}
		return;
	} else {
		*Eval = 30000;
		for (long i = 10; i <= 80; i += 10) {
			for (long j = 1; j <= 8; j++) {
				long c = Bord[i + j];
				if (c == '1') {
					char* NextBord = (char*)malloc(BORD_SIZE);
					memcpy(NextBord, Bord, BORD_SIZE);
					long i_max2, j_max2;
					if (Play(NextBord, Turn, i, j)) {
						free(NextBord);
						PeruseAll(NextBord, 1 - Turn, *Eval, 0, man, &Evaluation, &i_max2, &j_max2);
						if (Evaluation < *Eval) {
							*Eval = Evaluation;
							*i_max = i;
							*j_max = j;
						}
						if (Evaluation < CutValue) return;
					} else {
						free(NextBord);
					}
				}
			}
		}
		if (*Eval == 30000) {
			if (Pass) {
				if (man) {
					*Eval = -NumOfBlackMinusNumOfWhite(Bord);
				} else {
					*Eval = NumOfBlackMinusNumOfWhite(Bord);
				}
			} else {
				long i_max2, j_max2;
				PeruseAll(Bord, 1 - Turn, *Eval, 1, man, Eval, &i_max2, &j_max2);
			}
		}
		return;
	}
}
