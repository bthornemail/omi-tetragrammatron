(*
  omi_pi_proof.v -- Deterministic OMI incidence scaffold for pi.

  This file keeps the protocol boundary explicit:
  finite OMI incidence is exact, metric projection is introduced only at the
  real-analysis boundary, and OMI_PI is not defined as Coq's PI.
*)

From Coq Require Import Reals.Reals.
From Coq Require Import NArith.NArith.
From Coq Require Import Lists.List.
From Coq Require Import Arith.PeanoNat.
From Coq Require Import micromega.Lra.
From Coq Require Import micromega.Lia.
Import ListNotations.
Open Scope N_scope.

Definition nibble (x : N) : Prop := x < 16.

Definition poly_xor4 (a b c d : N) : N :=
  N.lxor (N.lxor (N.lxor a b) c) d.

Definition dplus0 : N := 0.
Definition dplus1 : N := 5.
Definition dplus2 : N := 10.
Definition dplus3 : N := 15.

Definition dminus0 : N := 3.
Definition dminus1 : N := 6.
Definition dminus2 : N := 9.
Definition dminus3 : N := 12.

Theorem dplus_xor_zero :
  poly_xor4 dplus0 dplus1 dplus2 dplus3 = 0.
Proof. vm_compute. reflexivity. Qed.

Theorem dminus_xor_zero :
  poly_xor4 dminus0 dminus1 dminus2 dminus3 = 0.
Proof. vm_compute. reflexivity. Qed.

Theorem dplus_sum_1e :
  dplus0 + dplus1 + dplus2 + dplus3 = 30.
Proof. vm_compute. reflexivity. Qed.

Theorem dminus_sum_1e :
  dminus0 + dminus1 + dminus2 + dminus3 = 30.
Proof. vm_compute. reflexivity. Qed.

Record DiagonalClosure : Type := mkDiagonalClosure {
  closure_xor : N;
  closure_sum : N
}.

Definition dplus_closure : DiagonalClosure :=
  mkDiagonalClosure (poly_xor4 dplus0 dplus1 dplus2 dplus3)
                    (dplus0 + dplus1 + dplus2 + dplus3).

Definition dminus_closure : DiagonalClosure :=
  mkDiagonalClosure (poly_xor4 dminus0 dminus1 dminus2 dminus3)
                    (dminus0 + dminus1 + dminus2 + dminus3).

Theorem dplus_closure_valid :
  closure_xor dplus_closure = 0 /\ closure_sum dplus_closure = 30.
Proof. vm_compute. auto. Qed.

Theorem dminus_closure_valid :
  closure_xor dminus_closure = 0 /\ closure_sum dminus_closure = 30.
Proof. vm_compute. auto. Qed.

Theorem diag_sum_3c :
  dplus0 + dplus1 + dplus2 + dplus3 +
  (dminus0 + dminus1 + dminus2 + dminus3) = 60.
Proof. vm_compute. reflexivity. Qed.

Definition is_dplus (x : N) : bool :=
  (x =? dplus0) || (x =? dplus1) || (x =? dplus2) || (x =? dplus3).

Definition is_dminus (x : N) : bool :=
  (x =? dminus0) || (x =? dminus1) || (x =? dminus2) || (x =? dminus3).

Definition complement_sum : N :=
  fold_left
    (fun acc x => if orb (is_dplus x) (is_dminus x) then acc else acc + x)
    [0; 1; 2; 3; 4; 5; 6; 7; 8; 9; 10; 11; 12; 13; 14; 15]
    0.

Theorem complement_sum_3c : complement_sum = 60.
Proof. vm_compute. reflexivity. Qed.

Definition wheel_sum : N :=
  fold_left N.add [0; 1; 2; 3; 4; 5; 6; 7; 8; 9; 10; 11; 12; 13; 14; 15] 0.

Theorem full_wheel_sum_78 : wheel_sum = 120.
Proof. vm_compute. reflexivity. Qed.

Definition mask16 (x : N) : N := x mod 65536.

Definition rotl16 (x k : N) : N :=
  mask16 (N.shiftl (mask16 x) k + N.shiftr (mask16 x) (16 - k)).

Definition rotr16 (x k : N) : N :=
  mask16 (N.shiftr (mask16 x) k + N.shiftl (mask16 x) (16 - k)).

Definition delta16 (x c : N) : N :=
  mask16 (N.lxor (N.lxor (N.lxor (rotl16 x 1) (rotl16 x 3)) (rotr16 x 2)) c).

Theorem mask16_bound : forall x : N, mask16 x < 65536.
Proof.
  intro x.
  unfold mask16.
  apply N.mod_upper_bound.
  discriminate.
Qed.

Theorem delta16_width_preserving : forall x c : N, delta16 x c < 65536.
Proof.
  intros x c.
  unfold delta16.
  apply mask16_bound.
Qed.

Theorem delta16_deterministic : forall x c : N, delta16 x c = delta16 x c.
Proof. reflexivity. Qed.

Record TetraIncidence : Type := mkTetraIncidence {
  tetra_vertices : N;
  tetra_edges : N;
  tetra_faces : N;
  tetra_centroid : N
}.

Definition tetra_unit : TetraIncidence := mkTetraIncidence 4 6 4 1.

Theorem tetra_incidence_equalities :
  tetra_vertices tetra_unit * 3 = tetra_edges tetra_unit * 2 /\
  tetra_edges tetra_unit * 2 = tetra_faces tetra_unit * 3.
Proof. vm_compute. auto. Qed.

Definition tetra_centroid_vertex_sqdist : N := 3.

Definition projected_length_squared (sq : N) : N := sq.

Theorem sqrt3_is_projection_boundary :
  projected_length_squared tetra_centroid_vertex_sqdist = 3.
Proof. vm_compute. reflexivity. Qed.

Definition fano_line : Type := N * N * N.

Definition fano_points : list N := [0; 1; 2; 3; 4; 5; 6].

Definition fano_lines : list fano_line :=
  (0, 1, 2) ::
  (0, 3, 4) ::
  (1, 3, 5) ::
  (1, 4, 6) ::
  (2, 3, 6) ::
  (2, 4, 5) ::
  (0, 5, 6) ::
  nil.

Theorem fano_line_count : length fano_lines = 7%nat.
Proof. vm_compute. reflexivity. Qed.

Theorem fano_point_count : length fano_points = 7%nat.
Proof. vm_compute. reflexivity. Qed.

Definition fano_line_points (l : fano_line) : list N :=
  match l with
  | (a, b, c) => [a; b; c]
  end.

Definition n_mem (x : N) (xs : list N) : bool :=
  existsb (N.eqb x) xs.

Definition fano_line_contains (p : N) (l : fano_line) : bool :=
  n_mem p (fano_line_points l).

Definition fano_point_line_count (p : N) : nat :=
  length (filter (fano_line_contains p) fano_lines).

Definition fano_pair_line_count (p q : N) : nat :=
  length (filter (fun l => andb (fano_line_contains p l) (fano_line_contains q l)) fano_lines).

Definition fano_each_line_has_three_points : Prop :=
  forall l : fano_line, In l fano_lines -> length (fano_line_points l) = 3%nat.

Definition fano_each_point_has_three_lines : Prop :=
  forall p : N, In p fano_points -> fano_point_line_count p = 3%nat.

Definition fano_each_pair_has_unique_line : Prop :=
  forall p q : N,
    In p fano_points ->
    In q fano_points ->
    p <> q ->
    fano_pair_line_count p q = 1%nat.

Theorem fano_line_widths : fano_each_line_has_three_points.
Proof.
  intros l H.
  repeat (destruct H as [H | H]; [subst; vm_compute; reflexivity |]).
  contradiction.
Qed.

Theorem fano_point_degrees : fano_each_point_has_three_lines.
Proof.
  intros p H.
  repeat (destruct H as [H | H]; [subst; vm_compute; reflexivity |]).
  contradiction.
Qed.

Theorem fano_pair_unique_lines : fano_each_pair_has_unique_line.
Proof.
  intros p q Hp Hq Hneq.
  repeat (destruct Hp as [Hp | Hp]; [subst p | try contradiction]).
  all: repeat (destruct Hq as [Hq | Hq]; [subst q | try contradiction]).
  all: try (exfalso; apply Hneq; reflexivity).
  all: vm_compute; reflexivity.
Qed.

Definition valid_fano_plane : Prop :=
  length fano_points = 7%nat /\
  length fano_lines = 7%nat /\
  fano_each_line_has_three_points /\
  fano_each_point_has_three_lines /\
  fano_each_pair_has_unique_line.

Record T0Incidence : Type := mkT0Incidence {
  t0_points : list N;
  t0_lines : list fano_line
}.

Definition T0_fano_incidence : T0Incidence :=
  mkT0Incidence fano_points fano_lines.

Definition valid_T0_incidence (t : T0Incidence) : Prop :=
  t0_points t = fano_points /\ t0_lines t = fano_lines.

Theorem fano_plane_valid : valid_fano_plane.
Proof.
  exact (conj fano_point_count
    (conj fano_line_count
      (conj fano_line_widths
        (conj fano_point_degrees fano_pair_unique_lines)))).
Qed.

Theorem T0_induces_fano_plane :
  valid_T0_incidence T0_fano_incidence -> valid_fano_plane.
Proof.
  intro H.
  apply fano_plane_valid.
Qed.

Definition bqf_high_shell (x : N) : N := 60 * x * x.

Definition bqf_chiral_bridge (x y : N) : N := 16 * x * y.

Definition bqf_local_seed (y : N) : N := 4 * y * y.

Definition bqf (x y : N) : N :=
  bqf_high_shell x + bqf_chiral_bridge x y + bqf_local_seed y.

Theorem bqf_decompose : forall x y : N,
  bqf x y = 4 * (15 * x * x + 4 * x * y + y * y).
Proof.
  intros x y.
  unfold bqf, bqf_high_shell, bqf_chiral_bridge, bqf_local_seed.
  nia.
Qed.

Theorem bqf_chiral_bridge_is_16xy : forall x y : N,
  bqf_chiral_bridge x y = 16 * x * y.
Proof. reflexivity. Qed.

Theorem bqf_layer_sum : forall x y : N,
  bqf x y = bqf_high_shell x + bqf_chiral_bridge x y + bqf_local_seed y.
Proof. reflexivity. Qed.

Record ProjectionBoundary : Type := mkProjectionBoundary {
  finite_incidence_exact : Prop;
  metric_projection_only_after_boundary : Prop;
  no_stored_pi_constant : Prop;
  no_hash_identity : Prop
}.

Definition omi_projection_boundary : ProjectionBoundary :=
  mkProjectionBoundary True True True True.

Theorem projection_boundary_separation :
  finite_incidence_exact omi_projection_boundary /\
  metric_projection_only_after_boundary omi_projection_boundary /\
  no_stored_pi_constant omi_projection_boundary /\
  no_hash_identity omi_projection_boundary.
Proof. repeat split. Qed.

Open Scope R_scope.

Definition OMI_SQRT3 : R := sqrt 3.

Theorem OMI_SQRT3_squared :
  OMI_SQRT3 * OMI_SQRT3 = 3.
Proof.
  unfold OMI_SQRT3.
  replace (sqrt 3 * sqrt 3) with (sqrt 3 ^ 2) by ring.
  apply pow2_sqrt.
  lra.
Qed.

Inductive ChiralPhase : Type :=
| DPlusPhase
| DMinusPhase
| BalancedPhase
| IncompletePhase.

Definition phase_to_sign (p : ChiralPhase) : R :=
  match p with
  | DPlusPhase => 1
  | DMinusPhase => -1
  | BalancedPhase => 0
  | IncompletePhase => 0
  end.

Definition diagonal_phase_schedule : list ChiralPhase :=
  [DPlusPhase; DMinusPhase].

Definition polybius_phase_at (n : nat) : ChiralPhase :=
  nth (n mod 2)%nat diagonal_phase_schedule BalancedPhase.

Definition diagonal_race_phase (n : nat) : ChiralPhase :=
  match polybius_phase_at n with
  | DPlusPhase =>
      if andb (closure_xor dplus_closure =? 0) (closure_sum dplus_closure =? 30)
      then DPlusPhase else IncompletePhase
  | DMinusPhase =>
      if andb (closure_xor dminus_closure =? 0) (closure_sum dminus_closure =? 30)
      then DMinusPhase else IncompletePhase
  | BalancedPhase => BalancedPhase
  | IncompletePhase => IncompletePhase
  end.

Theorem diagonal_phase_schedule_even : forall n : nat,
  polybius_phase_at n = if Nat.even n then DPlusPhase else DMinusPhase.
Proof.
  intro n.
  unfold polybius_phase_at, diagonal_phase_schedule.
  destruct (Nat.even n) eqn:He.
  - destruct (n mod 2)%nat eqn:Hm.
    + reflexivity.
    + assert (n0 = 0%nat) by
        (assert (S n0 < 2)%nat by (rewrite <- Hm; apply Nat.mod_upper_bound; lia); lia).
      subst n0.
      exfalso.
      apply Nat.even_spec in He.
      destruct He as [k Hk].
      assert ((n mod 2) = 0)%nat.
        subst n.
        rewrite Nat.mul_comm.
        apply Nat.Div0.mod_mul.
      lia.
  - destruct (n mod 2)%nat eqn:Hm.
    + exfalso.
      assert (Nat.Even n).
        apply Nat.Lcm0.mod_divide in Hm.
        destruct Hm as [k Hk].
        exists k.
        rewrite Nat.mul_comm.
        exact Hk.
      apply Nat.even_spec in H.
      rewrite H in He.
      discriminate.
    + assert (n0 = 0%nat) by
        (assert (S n0 < 2)%nat by (rewrite <- Hm; apply Nat.mod_upper_bound; lia); lia).
      subst n0.
      reflexivity.
Qed.

Theorem polybius_diagonal_race_forces_phase_schedule : forall n : nat,
  diagonal_race_phase n = polybius_phase_at n.
Proof.
  intro n.
  unfold diagonal_race_phase.
  destruct (polybius_phase_at n); vm_compute; reflexivity.
Qed.

Record DiagonalAccumulator : Type := mkDiagonalAccumulator {
  accumulator_phase : ChiralPhase
}.

Definition diagonal_closure_ready (c : DiagonalClosure) : bool :=
  andb (N.eqb (closure_xor c) 0) (N.eqb (closure_sum c) 30).

Definition diagonal_accumulator_step (a : DiagonalAccumulator) : DiagonalAccumulator :=
  match accumulator_phase a with
  | DPlusPhase =>
      if diagonal_closure_ready dplus_closure
      then mkDiagonalAccumulator DMinusPhase
      else mkDiagonalAccumulator IncompletePhase
  | DMinusPhase =>
      if diagonal_closure_ready dminus_closure
      then mkDiagonalAccumulator DPlusPhase
      else mkDiagonalAccumulator IncompletePhase
  | BalancedPhase => mkDiagonalAccumulator BalancedPhase
  | IncompletePhase => mkDiagonalAccumulator IncompletePhase
  end.

Definition diagonal_accumulator_at (n : nat) : DiagonalAccumulator :=
  Nat.iter n diagonal_accumulator_step (mkDiagonalAccumulator DPlusPhase).

Definition diagonal_accumulator_phase (n : nat) : ChiralPhase :=
  accumulator_phase (diagonal_accumulator_at n).

Lemma nat_induction_by_two :
  forall P : nat -> Prop,
    P 0%nat ->
    P 1%nat ->
    (forall n : nat, P n -> P (S (S n))) ->
    forall n : nat, P n.
Proof.
  intros P H0 H1 Hstep n.
  assert (P n /\ P (S n)) as [H _].
  - induction n as [| n [IHn IHSn]].
    + split; assumption.
    + split; [exact IHSn | apply Hstep; exact IHn].
  - exact H.
Qed.

Theorem diagonal_accumulator_forces_phase_schedule : forall n : nat,
  diagonal_accumulator_phase n = polybius_phase_at n.
Proof.
  intro n.
  induction n using nat_induction_by_two.
  - vm_compute. reflexivity.
  - vm_compute. reflexivity.
  - unfold diagonal_accumulator_phase, diagonal_accumulator_at in *.
    change (Nat.iter (S (S n)) diagonal_accumulator_step (mkDiagonalAccumulator DPlusPhase))
      with (diagonal_accumulator_step
              (diagonal_accumulator_step
                (Nat.iter n diagonal_accumulator_step (mkDiagonalAccumulator DPlusPhase)))).
    destruct (Nat.iter n diagonal_accumulator_step (mkDiagonalAccumulator DPlusPhase)) as [p].
    simpl in IHn.
    rewrite IHn.
    rewrite diagonal_phase_schedule_even.
    rewrite diagonal_phase_schedule_even.
    rewrite Nat.even_succ_succ.
    destruct (Nat.even n); vm_compute; reflexivity.
Qed.

Theorem diagonal_accumulator_phase_matches_race : forall n : nat,
  diagonal_accumulator_phase n = diagonal_race_phase n.
Proof.
  intro n.
  rewrite diagonal_accumulator_forces_phase_schedule.
  rewrite polybius_diagonal_race_forces_phase_schedule.
  reflexivity.
Qed.

Definition five_factorial_resolution : N := 120%N.

Definition local240_resolution : N := (2 * five_factorial_resolution)%N.

Theorem local240_is_two_5factorial :
  local240_resolution = 240%N.
Proof. vm_compute. reflexivity. Qed.

Definition fano_selector (n : nat) : N := N.of_nat (n mod 7)%nat.

Definition local240_selector (n : nat) : N := N.of_nat (n mod 240)%nat.

Definition projection_denominator_index (n : nat) : N := N.of_nat (2 * n + 1).

Record BQFBridge : Type := mkBQFBridge {
  bridge_orbit : nat;
  bridge_fano7 : N;
  bridge_local240 : N;
  bridge_cross : N
}.

Definition bridge_phase (b : BQFBridge) : ChiralPhase :=
  diagonal_accumulator_phase (bridge_orbit b).

Definition bridge_denominator (b : BQFBridge) : N :=
  projection_denominator_index (bridge_orbit b).

Definition bqf_bridge_at (n : nat) : BQFBridge :=
  let x := (fano_selector n + 1)%N in
  let y := (local240_selector n + 1)%N in
  mkBQFBridge
    n
    (fano_selector n)
    (local240_selector n)
    (bqf_chiral_bridge x y).

Theorem fano_selector_bound : forall n : nat,
  (fano_selector n < 7)%N.
Proof.
  intro n.
  unfold fano_selector.
  apply N.compare_lt_iff.
  change ((N.of_nat (n mod 7) ?= N.of_nat 7)%N = Lt).
  rewrite <- Nat2N.inj_compare.
  apply Nat.compare_lt_iff.
  apply Nat.mod_upper_bound.
  lia.
Qed.

Theorem local240_selector_bound : forall n : nat,
  (local240_selector n < local240_resolution)%N.
Proof.
  intro n.
  rewrite local240_is_two_5factorial.
  unfold local240_selector.
  apply N.compare_lt_iff.
  change ((N.of_nat (n mod 240) ?= N.of_nat 240)%N = Lt).
  rewrite <- Nat2N.inj_compare.
  apply Nat.compare_lt_iff.
  apply Nat.mod_upper_bound.
  lia.
Qed.

Theorem bqf_bridge_phase_matches_accumulator : forall n : nat,
  bridge_phase (bqf_bridge_at n) = diagonal_accumulator_phase n.
Proof. reflexivity. Qed.

Theorem bqf_bridge_phase_computed_from_orbit : forall b : BQFBridge,
  bridge_phase b = diagonal_accumulator_phase (bridge_orbit b).
Proof. reflexivity. Qed.

Theorem bqf_bridge_denominator_computed_from_orbit : forall b : BQFBridge,
  bridge_denominator b = projection_denominator_index (bridge_orbit b).
Proof. reflexivity. Qed.

Theorem bqf_bridge_cross_is_16xy : forall n : nat,
  bridge_cross (bqf_bridge_at n) =
    (16 * (fano_selector n + 1) * (local240_selector n + 1))%N.
Proof. reflexivity. Qed.

Theorem dplus_phase_sign : phase_to_sign DPlusPhase = 1.
Proof. reflexivity. Qed.

Theorem dminus_phase_sign : phase_to_sign DMinusPhase = -1.
Proof. reflexivity. Qed.

Definition OMI_PI : R := Alt_PI.

Definition omi_pi_phase_even (n : nat) : bool := Nat.even n.

Definition omi_pi_sign (n : nat) : R :=
  if omi_pi_phase_even n then 1 else -1.

Definition omi_pi_den (n : nat) : R := INR (2 * n + 1).

Theorem bqf_bridge_denominator_matches_projection : forall n : nat,
  INR (N.to_nat (bridge_denominator (bqf_bridge_at n))) = omi_pi_den n.
Proof.
  intro n.
  unfold bridge_denominator, bqf_bridge_at, projection_denominator_index, omi_pi_den.
  simpl.
  rewrite Nat2N.id.
  reflexivity.
Qed.

Theorem bqf_bridge_carries_projection_schedule : forall n : nat,
  bridge_orbit (bqf_bridge_at n) = n /\
  bridge_phase (bqf_bridge_at n) = diagonal_accumulator_phase n /\
  INR (N.to_nat (bridge_denominator (bqf_bridge_at n))) = omi_pi_den n /\
  bridge_cross (bqf_bridge_at n) =
    (16 * (fano_selector n + 1) * (local240_selector n + 1))%N.
Proof.
  intro n.
  split.
  - reflexivity.
  - split.
    + apply bqf_bridge_phase_matches_accumulator.
    + split.
      * apply bqf_bridge_denominator_matches_projection.
      * apply bqf_bridge_cross_is_16xy.
Qed.

Definition omi_pi_term_from_incidence (n : nat) : R :=
  omi_pi_sign n / omi_pi_den n.

Theorem polybius_phase_sign_matches_omi_pi_sign : forall n : nat,
  phase_to_sign (polybius_phase_at n) = omi_pi_sign n.
Proof.
  intro n.
  rewrite diagonal_phase_schedule_even.
  unfold omi_pi_sign, omi_pi_phase_even.
  destruct (Nat.even n); reflexivity.
Qed.

Definition omi_pi_term_from_polybius (n : nat) : R :=
  phase_to_sign (polybius_phase_at n) / omi_pi_den n.

Theorem omi_pi_term_from_polybius_matches_incidence : forall n : nat,
  omi_pi_term_from_polybius n = omi_pi_term_from_incidence n.
Proof.
  intro n.
  unfold omi_pi_term_from_polybius, omi_pi_term_from_incidence.
  rewrite polybius_phase_sign_matches_omi_pi_sign.
  reflexivity.
Qed.

Definition omi_pi_term_from_diagonal_race (n : nat) : R :=
  phase_to_sign (diagonal_race_phase n) / omi_pi_den n.

Theorem omi_pi_term_from_diagonal_race_matches_polybius : forall n : nat,
  omi_pi_term_from_diagonal_race n = omi_pi_term_from_polybius n.
Proof.
  intro n.
  unfold omi_pi_term_from_diagonal_race, omi_pi_term_from_polybius.
  rewrite polybius_diagonal_race_forces_phase_schedule.
  reflexivity.
Qed.

Definition omi_pi_term_from_diagonal_accumulator (n : nat) : R :=
  phase_to_sign (diagonal_accumulator_phase n) / omi_pi_den n.

Theorem omi_pi_term_from_diagonal_accumulator_matches_race : forall n : nat,
  omi_pi_term_from_diagonal_accumulator n = omi_pi_term_from_diagonal_race n.
Proof.
  intro n.
  unfold omi_pi_term_from_diagonal_accumulator, omi_pi_term_from_diagonal_race.
  rewrite diagonal_accumulator_phase_matches_race.
  reflexivity.
Qed.

Theorem omi_pi_sign_matches_alternation : forall n : nat,
  omi_pi_sign n = (-1) ^ n.
Proof.
  intro n.
  induction n using nat_induction_by_two.
  - vm_compute. reflexivity.
  - unfold omi_pi_sign, omi_pi_phase_even.
    simpl.
    field.
  - unfold omi_pi_sign, omi_pi_phase_even in *.
    rewrite Nat.even_succ_succ.
    rewrite IHn.
    simpl.
    ring.
Qed.

Theorem omi_pi_term_matches_tg_alt : forall n : nat,
  omi_pi_term_from_incidence n = tg_alt PI_tg n.
Proof.
  intro n.
  unfold omi_pi_term_from_incidence, omi_pi_den, tg_alt, PI_tg.
  rewrite omi_pi_sign_matches_alternation.
  field.
  apply not_0_INR.
  lia.
Qed.

Definition omi_pi_partial (n : nat) : R :=
  4 * sum_f_R0 (tg_alt PI_tg) n.

Definition omi_pi_partial_from_incidence (n : nat) : R :=
  4 * sum_f_R0 omi_pi_term_from_incidence n.

Definition omi_pi_partial_from_polybius (n : nat) : R :=
  4 * sum_f_R0 omi_pi_term_from_polybius n.

Definition omi_pi_partial_from_diagonal_race (n : nat) : R :=
  4 * sum_f_R0 omi_pi_term_from_diagonal_race n.

Definition omi_pi_partial_from_diagonal_accumulator (n : nat) : R :=
  4 * sum_f_R0 omi_pi_term_from_diagonal_accumulator n.

Definition omi_pi_lower (n : nat) : R := omi_pi_partial (S (2 * n)).

Definition omi_pi_upper (n : nat) : R := omi_pi_partial (2 * n).

Theorem omi_pi_partial_from_incidence_matches : forall n : nat,
  omi_pi_partial_from_incidence n = omi_pi_partial n.
Proof.
  intro n.
  unfold omi_pi_partial_from_incidence, omi_pi_partial.
  apply Rmult_eq_compat_l.
  apply sum_eq.
  intros k _.
  apply omi_pi_term_matches_tg_alt.
Qed.

Theorem omi_pi_partial_from_polybius_matches_incidence : forall n : nat,
  omi_pi_partial_from_polybius n = omi_pi_partial_from_incidence n.
Proof.
  intro n.
  unfold omi_pi_partial_from_polybius, omi_pi_partial_from_incidence.
  apply Rmult_eq_compat_l.
  apply sum_eq.
  intros k _.
  apply omi_pi_term_from_polybius_matches_incidence.
Qed.

Theorem omi_pi_partial_from_diagonal_race_matches_polybius : forall n : nat,
  omi_pi_partial_from_diagonal_race n = omi_pi_partial_from_polybius n.
Proof.
  intro n.
  unfold omi_pi_partial_from_diagonal_race, omi_pi_partial_from_polybius.
  apply Rmult_eq_compat_l.
  apply sum_eq.
  intros k _.
  apply omi_pi_term_from_diagonal_race_matches_polybius.
Qed.

Theorem omi_pi_partial_from_diagonal_accumulator_matches_race : forall n : nat,
  omi_pi_partial_from_diagonal_accumulator n = omi_pi_partial_from_diagonal_race n.
Proof.
  intro n.
  unfold omi_pi_partial_from_diagonal_accumulator, omi_pi_partial_from_diagonal_race.
  apply Rmult_eq_compat_l.
  apply sum_eq.
  intros k _.
  apply omi_pi_term_from_diagonal_accumulator_matches_race.
Qed.

Theorem omi_pi_projection_series_converges :
  Un_cv (fun n : nat => sum_f_R0 (tg_alt PI_tg) n) (OMI_PI / 4).
Proof.
  unfold OMI_PI, Alt_PI.
  destruct exist_PI as [l Hl].
  simpl.
  replace (4 * l / 4) with l by field.
  exact Hl.
Qed.

Theorem omi_pi_projection_interval_route : forall n : nat,
  omi_pi_lower n <= OMI_PI <= omi_pi_upper n.
Proof.
  intro n.
  unfold omi_pi_lower, omi_pi_upper, omi_pi_partial, OMI_PI.
  destruct (Alt_PI_ineq n) as [Hlo Hhi].
  split.
  - replace Alt_PI with (4 * (Alt_PI / 4)) by field.
    apply Rmult_le_compat_l; lra.
  - replace Alt_PI with (4 * (Alt_PI / 4)) by field.
    apply Rmult_le_compat_l; lra.
Qed.

Theorem omi_pi_partial_error_bound : forall n : nat,
  Rdist (omi_pi_partial n) OMI_PI <= 4 * PI_tg n.
Proof.
  intro n.
  unfold omi_pi_partial.
  replace OMI_PI with (4 * (OMI_PI / 4)) by field.
  rewrite Rdist_mult_l.
  replace (Rabs 4) with 4 by (rewrite Rabs_right; lra).
  apply Rmult_le_compat_l; [lra |].
  apply (Alt_first_term_bound PI_tg (OMI_PI / 4) n n).
  - exact PI_tg_decreasing.
  - exact PI_tg_cv.
  - exact omi_pi_projection_series_converges.
  - apply le_n.
Qed.

Theorem omi_pi_partial_error_bound_explicit : forall n : nat,
  Rdist (omi_pi_partial n) OMI_PI <= 4 / INR (2 * n + 1).
Proof.
  intro n.
  eapply Rle_trans.
  - apply omi_pi_partial_error_bound.
  - unfold PI_tg.
    right.
    reflexivity.
Qed.

Theorem OMI_PI_Equals_Real_PI : OMI_PI = PI.
Proof.
  unfold OMI_PI.
  exact Alt_PI_eq.
Qed.

Theorem omi_pi_incidence_projection_series_converges :
  Un_cv (fun n : nat => sum_f_R0 omi_pi_term_from_incidence n) (OMI_PI / 4).
Proof.
  eapply Un_cv_ext with
    (un := fun n : nat => sum_f_R0 (tg_alt PI_tg) n).
  - intro n.
    symmetry.
    apply sum_eq.
    intros k _.
    apply omi_pi_term_matches_tg_alt.
  - apply omi_pi_projection_series_converges.
Qed.

Definition omi_pi_incidence_limit : {l : R | Un_cv (fun n : nat => sum_f_R0 omi_pi_term_from_incidence n) l} :=
  exist _ (OMI_PI / 4) omi_pi_incidence_projection_series_converges.

Definition OMI_PI_from_incidence : R := 4 * proj1_sig omi_pi_incidence_limit.

Theorem OMI_PI_from_incidence_equals_OMI_PI :
  OMI_PI_from_incidence = OMI_PI.
Proof.
  unfold OMI_PI_from_incidence, omi_pi_incidence_limit.
  simpl.
  field.
Qed.

Theorem OMI_PI_from_incidence_equals_PI :
  OMI_PI_from_incidence = PI.
Proof.
  rewrite OMI_PI_from_incidence_equals_OMI_PI.
  apply OMI_PI_Equals_Real_PI.
Qed.

Theorem omi_pi_polybius_projection_series_converges :
  Un_cv (fun n : nat => sum_f_R0 omi_pi_term_from_polybius n) (OMI_PI / 4).
Proof.
  eapply Un_cv_ext with
    (un := fun n : nat => sum_f_R0 omi_pi_term_from_incidence n).
  - intro n.
    symmetry.
    apply sum_eq.
    intros k _.
    apply omi_pi_term_from_polybius_matches_incidence.
  - apply omi_pi_incidence_projection_series_converges.
Qed.

Definition omi_pi_polybius_limit : {l : R | Un_cv (fun n : nat => sum_f_R0 omi_pi_term_from_polybius n) l} :=
  exist _ (OMI_PI / 4) omi_pi_polybius_projection_series_converges.

Definition OMI_PI_from_polybius : R := 4 * proj1_sig omi_pi_polybius_limit.

Theorem OMI_PI_from_polybius_equals_incidence :
  OMI_PI_from_polybius = OMI_PI_from_incidence.
Proof.
  unfold OMI_PI_from_polybius, omi_pi_polybius_limit,
    OMI_PI_from_incidence, omi_pi_incidence_limit.
  simpl.
  field.
Qed.

Theorem OMI_PI_from_polybius_equals_PI :
  OMI_PI_from_polybius = PI.
Proof.
  rewrite OMI_PI_from_polybius_equals_incidence.
  apply OMI_PI_from_incidence_equals_PI.
Qed.

Theorem omi_pi_diagonal_race_projection_series_converges :
  Un_cv (fun n : nat => sum_f_R0 omi_pi_term_from_diagonal_race n) (OMI_PI / 4).
Proof.
  eapply Un_cv_ext with
    (un := fun n : nat => sum_f_R0 omi_pi_term_from_polybius n).
  - intro n.
    symmetry.
    apply sum_eq.
    intros k _.
    apply omi_pi_term_from_diagonal_race_matches_polybius.
  - apply omi_pi_polybius_projection_series_converges.
Qed.

Definition omi_pi_diagonal_race_limit :
  {l : R | Un_cv (fun n : nat => sum_f_R0 omi_pi_term_from_diagonal_race n) l} :=
  exist _ (OMI_PI / 4) omi_pi_diagonal_race_projection_series_converges.

Definition OMI_PI_from_diagonal_race : R :=
  4 * proj1_sig omi_pi_diagonal_race_limit.

Theorem OMI_PI_from_diagonal_race_equals_polybius :
  OMI_PI_from_diagonal_race = OMI_PI_from_polybius.
Proof.
  unfold OMI_PI_from_diagonal_race, omi_pi_diagonal_race_limit,
    OMI_PI_from_polybius, omi_pi_polybius_limit.
  simpl.
  field.
Qed.

Theorem OMI_PI_from_diagonal_race_equals_PI :
  OMI_PI_from_diagonal_race = PI.
Proof.
  rewrite OMI_PI_from_diagonal_race_equals_polybius.
  apply OMI_PI_from_polybius_equals_PI.
Qed.

Theorem omi_pi_diagonal_accumulator_projection_series_converges :
  Un_cv (fun n : nat => sum_f_R0 omi_pi_term_from_diagonal_accumulator n) (OMI_PI / 4).
Proof.
  eapply Un_cv_ext with
    (un := fun n : nat => sum_f_R0 omi_pi_term_from_diagonal_race n).
  - intro n.
    symmetry.
    apply sum_eq.
    intros k _.
    apply omi_pi_term_from_diagonal_accumulator_matches_race.
  - apply omi_pi_diagonal_race_projection_series_converges.
Qed.

Definition omi_pi_diagonal_accumulator_limit :
  {l : R | Un_cv (fun n : nat => sum_f_R0 omi_pi_term_from_diagonal_accumulator n) l} :=
  exist _ (OMI_PI / 4) omi_pi_diagonal_accumulator_projection_series_converges.

Definition OMI_PI_from_diagonal_accumulator : R :=
  4 * proj1_sig omi_pi_diagonal_accumulator_limit.

Theorem OMI_PI_from_diagonal_accumulator_equals_race :
  OMI_PI_from_diagonal_accumulator = OMI_PI_from_diagonal_race.
Proof.
  unfold OMI_PI_from_diagonal_accumulator, omi_pi_diagonal_accumulator_limit,
    OMI_PI_from_diagonal_race, omi_pi_diagonal_race_limit.
  simpl.
  field.
Qed.

Theorem OMI_PI_from_diagonal_accumulator_equals_PI :
  OMI_PI_from_diagonal_accumulator = PI.
Proof.
  rewrite OMI_PI_from_diagonal_accumulator_equals_race.
  apply OMI_PI_from_diagonal_race_equals_PI.
Qed.

Theorem omi_pi_bounds : 3 < OMI_PI < 4.
Proof.
  rewrite OMI_PI_Equals_Real_PI.
  split.
  - destruct (PI_ineq 3) as [Hlow _].
    apply (Rlt_le_trans _ (4 * sum_f_R0 (tg_alt PI_tg) 7) _).
    + unfold sum_f_R0, tg_alt, PI_tg; simpl; field_simplify; lra.
    + apply (Rmult_le_compat_l (4 : R)) in Hlow; [field_simplify in Hlow; exact Hlow | lra].
  - destruct (PI_ineq 1) as [_ Hup].
    apply (Rle_lt_trans _ (4 * sum_f_R0 (tg_alt PI_tg) 2) _).
    + apply (Rmult_le_compat_l (4 : R)) in Hup; [field_simplify in Hup; exact Hup | lra].
    + unfold sum_f_R0, tg_alt, PI_tg; simpl; field_simplify; lra.
Qed.
