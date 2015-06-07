import json

METHOD_ARG_MAP = {
	'webPlace_cinv':
			(
					('srcpom', int), ('srcpon', int), ('srcmany', int), ('dstpom', int), ('dstpon', int),
			),
	'webUpdate_cinv':
			(
					('pov', int), ('idx', int),
			),
	'webDrop_cinv':
			(
					('pom', int), ('pon', int), ('many', int),
			),

	'webPlace_einv':(
					('src', int), ('dst', int),
			),
	'webRig_einv':(
					('pom', int), ('pon', int), ('pov', int),
			),
	'webUnrig_einv':(
					('pov', int), ('pom', int), ('pon', int),
			),

	'webSynthesizea_stone':(
					('daopom', int), ('daopon', int), ('poitup', json.loads),
			),
	'webSynthesizeb_stone':(
					('pom1', int), ('pon1', int), ('pom2', int), ('pon2', int), ('pom3', int), ('pon3', int),
			),
	'webPro_stone':(
					('srcpom', int), ('srcpon', int), ('daopom', int), ('daopon', int),
			),
	'webPunch_stone':(
					('srcpom', int), ('srcpon', int), ('dstpom', int), ('dstpon', int),
			),
	'webInlaya_stone':(
					('dstpom', int), ('dstpon', int), ('inlaydct', json.loads),
			),
	'webInlayb_stone':(
					('dstpom', int), ('dstpon', int), ('inlaydct', json.loads),
			),
	'webUninlay_stone':(
					('srcpom', int), ('srcpon', int), ('dstpom', int), ('dstpon', int), ('hole', int),
			),

	'webForging_equip':(
					('equpom', int), ('equpon', int), ('spom', int), ('spon', int), ('bpom', int), ('bpon', int),
			),
	'webMovestar_equip':(
					('equpom', int), ('equpon', int), ('spom', int), ('spon', int), ('bpom', int), ('bpon', int),
			),
	'webLucky_equip':(
					('equpom', int), ('equpon', int), ('bpom', int), ('bpon', int),
			),
	'webImprint_equip':(
					('srcpom', int), ('srcpon', int), ('dstpom', int), ('dstpon', int),
			),

	'webPro_intensify':(
					('protype', int), ('srcpom', int), ('srcpon', int), ('dstpom', int), ('dstpon', int), ('dx', int), ('ppnum', int), ('pplist', json.loads), ('pprand', int),
			),
	'webUp_intensify':(
					('daopom', int), ('daopon', int), ('srcpom', int),
			),
	'webMovepp_intensify':(
					('daopom', int), ('daopon', int), ('srcpom', int), ('dstpom', int), ('dstpon', int),
			),

	'webPro_shenqi':(
					('equpom', int), ('equpon', int), ('tupom', int), ('tupon', int), ('daopom', int), ('daopon', int),
			),
	'webYelian_shenqi':(
					('equpom', int), ('equpon', int), ('ppno', int), ('daopom', int), ('daopon', int),
			),
	'webChongzhi_shenqi':(
					('equpom', int), ('equpon', int), ('pprand', json.loads), ('daopom', int), ('daopon', int),
			),
}
