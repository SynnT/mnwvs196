#pragma once
#include <atomic>
#include "..\WvsLib\DateTime\GameDateTime.h"
#include "..\WvsLib\Net\InPacket.h"
#include "..\WvsLib\Net\OutPacket.h"

struct GW_ItemSlotBase
{
	typedef long long int ATOMIC_COUNT_TYPE;

	enum GW_ItemSlotType
	{
		EQUIP,
		CONSUME,
		ETC,
		INSTALL,
		CASH
	};

	enum GW_ItemSlotStatus
	{
		INVENTORY,
		DROPPED,
		SOLD,
	};

	static ATOMIC_COUNT_TYPE InitItemSN(GW_ItemSlotType type);

	static ATOMIC_COUNT_TYPE IncItemSN(GW_ItemSlotType type)
	{
		static std::atomic<ATOMIC_COUNT_TYPE> eqpAtomicCounter = InitItemSN(GW_ItemSlotType::EQUIP);
		static std::atomic<ATOMIC_COUNT_TYPE> conAtomicCounter = InitItemSN(GW_ItemSlotType::CONSUME);
		static std::atomic<ATOMIC_COUNT_TYPE> insAtomicCounter = InitItemSN(GW_ItemSlotType::INSTALL);
		static std::atomic<ATOMIC_COUNT_TYPE> etcAtomicCounter = InitItemSN(GW_ItemSlotType::ETC);
		if (type == GW_ItemSlotType::EQUIP)
			return ++eqpAtomicCounter;
		else if (type == GW_ItemSlotType::CONSUME)
			return ++conAtomicCounter;
		else if (type == GW_ItemSlotType::INSTALL)
			return ++insAtomicCounter;
		else if (type == GW_ItemSlotType::ETC)
			return ++etcAtomicCounter;
	}

	GW_ItemSlotType nType;
	GW_ItemSlotStatus nStatus = GW_ItemSlotStatus::INVENTORY;
	short nPOS = 0;

	int nItemID = 0,
		nAttribute = 0,
		nCharacterID = -1;

	long long int liExpireDate = GameDateTime::TIME_UNLIMITED,
				  liItemSN = -1,
				  liCashItemSN = -1;

	bool bIsInBag = false, bIsPet = false;

public:
	GW_ItemSlotBase();
	~GW_ItemSlotBase();

	virtual void Encode(OutPacket *oPacket, bool bForInternal) const = 0;
	virtual void RawEncode(OutPacket *oPacket) const = 0;

	void DecodeItemSlot(InPacket *iPacket, GW_ItemSlotType type);
	virtual void Decode(InPacket *iPacket, bool bForInternal) = 0;
	virtual void RawDecode(InPacket *iPacket) = 0;

	void EncodeInventoryPosition(OutPacket *oPacket) const;
	void EncodeTradingPosition(OutPacket *oPacket) const;

	void DecodeInventoryPosition(InPacket *iPacket) const;

	virtual GW_ItemSlotBase* MakeClone() = 0;

	//檢查整個物品是否視為單一物件
	bool IsTreatSingly() const;
};

