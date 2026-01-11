#pragma once
/*virtual __int64 prepareRegion(__int64);                                                              // 357
virtual __int64 destroyRegion(void);                                                                 // 358
virtual __int64 suspendRegion(void);                                                                 // 359
virtual __int64 resendAllChunks(void);                                                               // 360
virtual __int64 _fireWillChangeDimension(void);                                                      // 361 emote
virtual __int64 _fireDimensionChanged(void);                                                         // 362
virtual __int64 changeDimensionWithCredits(int);                                                     // 363
virtual __int64 tickWorld(int const&);                                                              // 364
virtual __int64 frameUpdate(__int64);                                                                // 365
virtual __int64 getTickingOffsets(void);                                                             // 366
virtual __int64 moveView(void);                                                                      // 367
virtual __int64 setName(TextHolder*);                                                               // 368
virtual __int64 checkMovementStats(vec3_t const&);                                                  // 369
virtual __int64 getCurrentStructureFeature(void);                                                    // 370
virtual bool isAutoJumpEnabled(void);                                                                // 371
virtual __int64 respawn(void);                                                                       // 372
virtual __int64 resetRot(void);                                                                      // 373
virtual __int64 resetPos(bool);                                                                      // 374
virtual bool isInTrialMode(void);                                                                    // 375
virtual bool hasResource(int);                                                                       // 376
virtual __int64 completeUsingC_Item(void);                                                           // 377
virtual __int64 setPermissions(void);                                                                // 378
virtual __int64 startDestroying(void);                                                               // 379
virtual __int64 stopDestroying(void);                                                                // 380
virtual __int64 openPortfolio(void);                                                                 // 381
virtual __int64 openBook(int, bool, int, __int64*);                                                 // 382
virtual __int64 openTrading(__int64 const&, bool);                                                  // 383
virtual bool canOpenContainerScreen(void);                                                           // 384
virtual __int64 openChalkboard(__int64*, bool);                                                     // 385
virtual __int64 openNpcInteractScreen(std::shared_ptr<__int64>);                                     // 386
virtual __int64 openInventory(void);                                                                 // 387
virtual __int64 displayChatMessage(TextHolder*, TextHolder*);                                      // 388
virtual __int64 displayClientMessage(TextHolder*);                                                  // 389
virtual __int64 displayTextObjectMessage(__int64 const&, TextHolder*, TextHolder*);               // 390
virtual __int64 displayTextObjectWhisperMessage(__int64 const&, TextHolder*, TextHolder*);        // 391
virtual __int64 displayWhisperMessage(TextHolder*, TextHolder*, TextHolder*, TextHolder*);       // 392
virtual __int64 startSleepInBed(vec3_ti const&);                                                    // 393
virtual __int64 stopSleepInBed(bool, bool);                                                          // 394
virtual bool canStartSleepInBed(void);                                                               // 395
virtual __int64 getSleepTimer(void);                                                                 // 396
virtual __int64 getPreviousTickSleepTimer(void);                                                     // 397
virtual __int64 openSign(vec3_ti const&);                                                           // 398
virtual void playEmote(std::string);                                                                 // 399
virtual bool isHostingC_Player(void);                                                                // 400
virtual bool isLoading(void);                                                                        // 401
virtual bool isC_PlayerInitialized(void);                                                            // 402
virtual __int64 stopLoading(void);                                                                   // 403
virtual __int64 registerTrackedBoss(__int64);                                                        // 404
virtual __int64 unRegisterTrackedBoss(__int64);                                                      // 405
virtual __int64 setC_PlayerGameType(int);                                                            // 406
virtual __int64 initHUDContainerManager(void);                                                       // 407
virtual __int64 _crit(C_Entity*);                                                                   // 408
virtual __int64 getEventing(void);                                                                   // 409
virtual __int64 getUserId(void);                                                                     // 410
virtual __int64 sendEventPacket(__int64);                                                            // 411
virtual __int64 addExperience(int);                                                                  // 412
virtual __int64 addLevels(int);                                                                      // 413
virtual __int64 setContainerData(__int64, int, int);                                                 // 414
virtual __int64 slotChanged(__int64, __int64, int, C_ItemStack const&, C_ItemStack const&, bool);  // 415
virtual __int64 inventoryChanged(__int64, int, C_ItemStack const&, C_ItemStack const&, bool);      // 416
virtual __int64 refreshContainer(__int64);                                                           // 417
virtual __int64 deleteContainerManager(void);                                                        // 418
virtual __int64 setFieldOfViewModifier(float);                                                       // 419
virtual bool is2DPositionRelevant(int, vec3_ti const&);                                             // 420
virtual bool isActorRelevant(C_Entity*);                                                            // 421
virtual bool isTeacher(void);                                                                        // 422
virtual __int64 onSuspension(void);                                                                  // 423
virtual __int64 onLinkedSlotsChanged(void);                                                          // 424
virtual __int64 startCooldown(C_Item const*);                                                       // 425
virtual __int64 getC_ItemCooldownLeft();                                                             // 426
virtual __int64 getC_ItemCooldownLeft(unsigned long);                                                // 427
virtual __int64 getMaxC_ItemCooldownLeft(void);                                                      // 428
virtual bool isC_ItemInCooldown();                                                                   // 429
virtual __int64 sendInventoryTransaction(C_InventoryTransaction const&);                            // 430
virtual __int64 sendComplexInventoryTransaction(__int64);                                            // 431
virtual __int64 sendNetworkPacket(class C_Packet&);                                                 // 432
virtual __int64 getC_PlayerEventCoordinator(void);                                                   // 433
virtual __int64 getMoveInputHandler(void);                                                           // 434
virtual __int64 getInputMode(void);                                                                  // 435
virtual __int64 getPlayMode(void);                                                                   // 436
virtual __int64 reportMovementTelemetry(int);                                                        // 437
virtual bool isSimulated(void);                                                                      // 438
virtual __int64 getXuid(void);                                                                       // 439
virtual __int64 getMovementSettings(void);                                                           // 440
virtual __int64 onMoveC_PlayerPacketNormal(vec3_t const&, vec2_t const&, float);                   // 441
virtual __int64 _createChunkSource();                                                                // 442
virtual __int64 _getSpawnChunkLimit(void);                                                           // 443
virtual __int64 _updateChunkPublisherView(vec3_t const&, float);                                    // */